#include "BatchByMaterialRenderProcessor.h"
#include "MaterialRenderModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderPassModel.h"

using namespace std;
using namespace PJ;

namespace {
    /// Replace `parent`'s children with the provided ordered list of replacement nodes.
    /// AcyclicGraphNode doesn't expose reparenting, so clear and re-add in order.
    void ReplaceChildren(
        SomeRenderModelNode& parent, VectorList<RenderPassModel::NodeSharedPtr> const& replacements
    ) {
        parent.RemoveAllEdges();
        for (auto& child : replacements) {
            if (child) {
                parent.AddEdge(child);
            }
        }
    }
} // namespace

void BatchByMaterialRenderProcessor::Process(SomeRenderModelNode& rootNode) {
    BatchChildren(rootNode);
}

void BatchByMaterialRenderProcessor::BatchChildren(SomeRenderModelNode& parent) {
    // Snapshot the child nodes up front — we rebuild the edge list in place below.
    VectorList<RenderPassModel::NodeSharedPtr> originalChildren;
    for (auto& edge : parent.Edges()) {
        if (!edge || !edge->toNode) {
            continue;
        }
        auto child = edge->toNode->Value();
        if (!child) {
            continue;
        }
        originalChildren.push_back(SCAST<RenderPassModel::Node>(child));
    }

    VectorList<RenderPassModel::NodeSharedPtr> result;
    result.reserve(originalChildren.size());

    VectorList<RenderPassModel::NodeSharedPtr> runNodes;
    VectorList<MaterialRenderModel*> runModels;
    RenderMaterial* runMaterial{};
    String runKey;
    bool isRunBlended = false;

    auto flushRun = [&]() {
        if (runModels.empty()) {
            return;
        }
        if (runModels.size() == 1) {
            // Nothing to combine; keep the original node intact so descendants are
            // preserved. (A lone material won't have material descendants in a well-formed
            // DAG, but better to keep the structure than risk losing anything.)
            result.push_back(runNodes.front());
        } else {
            auto combined = Combine(runModels);
            if (combined) {
                auto spCombined = MAKE<MaterialRenderModel>(*combined);
                auto combinedNode = MAKE<RenderPassModel::Node>();
                combinedNode->core = spCombined;
                spCombined->node = combinedNode.get();
                result.push_back(combinedNode);
            }
        }
        runNodes.clear();
        runModels.clear();
        runMaterial = nullptr;
        runKey.clear();
        isRunBlended = false;
    };

    for (auto& child : originalChildren) {
        auto* model = dynamic_cast<MaterialRenderModel*>(child->core.get());

        if (!model) {
            // Non-material node — break the run, preserve the node, recurse into its
            // subtree so nested runs (e.g. under a stencil push) batch too.
            flushRun();
            BatchChildren(*child);
            result.push_back(child);
            continue;
        }

        // A material with its own children is treated as a scope boundary — we don't batch
        // it away because something hangs off it. Flush, keep the node, recurse.
        if (!child->Edges().empty()) {
            flushRun();
            BatchChildren(*child);
            result.push_back(child);
            continue;
        }

        auto* renderMaterial = model->Material();

        // A MaterialRenderModel with no RenderMaterial can't share render state with
        // anything else — keep it as a singleton.
        if (!renderMaterial) {
            flushRun();
            result.push_back(child);
            continue;
        }

        String key = renderMaterial->PropertyId();
        bool const isBlended = model->IsFeatureEnabled(RenderFeature::Blend);

        if (runModels.empty()) {
            runMaterial = renderMaterial;
            runKey = key;
            isRunBlended = isBlended;
        } else if (renderMaterial != runMaterial &&
                   (key != runKey || key.empty() || isBlended != isRunBlended)) {
            // Different RenderMaterial pointer AND either a different propertyId, an empty
            // (untracked) propertyId, or a blend-mode mismatch — start a new run.
            flushRun();
            runMaterial = renderMaterial;
            runKey = key;
            isRunBlended = isBlended;
        }

        runNodes.push_back(child);
        runModels.push_back(model);
    }
    flushRun();

    ReplaceChildren(parent, result);
}

std::optional<MaterialRenderModel>
BatchByMaterialRenderProcessor::Combine(VectorList<MaterialRenderModel*>& renderModels) {
    GUARDR(!IsEmpty(renderModels), {})

    if (!renderModels[0]->IsFeatureEnabled(RenderFeature::Blend)) {
        for (auto& modelP : renderModels) {
            auto& model = *modelP;
            // Force the z order of opaque objects for depth buffer optimization
            std::for_each(
                model.ModifiableMesh().Vertices().begin(), model.ModifiableMesh().Vertices().end(),
                [&](Vector3& vertex) { vertex.z = model.z; }
            );
        }
    }

    // Optimize: Skip process if we have nothing to batch
    if (renderModels.size() == 1) {
        return *renderModels[0];
    }

    // Make sure everything is in world coordinates before we batch
    for (auto& model : renderModels) {
        model->ModifiableMesh() *= model->matrix;
        model->matrix.LoadIdentity();
    }

    size_t vertexCount = 0;
    size_t trianglesCount = 0;
    size_t uvsCount = 0;
    size_t colorsCount = 0;

    for (auto& modelP : renderModels) {
        auto& model = *modelP;
        vertexCount += model.GetMesh().Vertices().size();
        trianglesCount += model.GetMesh().Triangles().size();
        uvsCount += model.GetMesh().UVs().size();

        // 1 color per vertex
        colorsCount += model.GetMesh().Vertices().size();

        auto thisColorsCount = model.VertexColors().size();
        auto thisVertexCount = model.GetMesh().Vertices().size();
        if (thisColorsCount != thisVertexCount) {
            PJ::Log(
                "WARNING: colorsCount ", (int)thisColorsCount, " and vertexCount ",
                (int)thisVertexCount, " don't match"
            );
        }
    }

    MaterialRenderModel renderModel = *renderModels[0];
    VectorList<Vector3> vertices;
    vertices.resize(vertexCount);
    renderModel.ModifiableMesh().SetVertices(vertices);
    renderModel.ModifiableMesh().Triangles().resize(trianglesCount);

    VectorList<Vector2> uvs;
    uvs.resize(uvsCount);
    renderModel.ModifiableMesh().SetUVs(uvs);
    renderModel.ModifiableVertexColors().resize(colorsCount);

    size_t vertexIndex = 0;
    size_t triangleIndex = 0;
    size_t uvIndex = 0;
    size_t colorIndex = 0;

    for (auto& modelP : renderModels) {
        auto& model = *modelP;
        auto modelVertexCount = model.GetMesh().Vertices().size();
        auto modelTriangleCount = model.GetMesh().Triangles().size();
        auto modelUVCount = model.GetMesh().UVs().size();
        auto modelColorCount = model.VertexColors().size();

        std::copy_n(
            model.GetMesh().Vertices().cbegin(), modelVertexCount,
            renderModel.ModifiableMesh().Vertices().begin() + vertexIndex
        );
        std::copy_n(
            model.GetMesh().UVs().cbegin(), modelUVCount,
            renderModel.ModifiableMesh().UVs().begin() + uvIndex
        );
        std::copy_n(
            model.GetMesh().Triangles().cbegin(), modelTriangleCount,
            renderModel.ModifiableMesh().Triangles().begin() + triangleIndex
        );

        // Max: 1 color per vertex
        std::copy_n(
            model.VertexColors().begin(), std::min(modelColorCount, modelVertexCount),
            renderModel.ModifiableVertexColors().begin() + colorIndex
        );

        for (int i = 0; i < modelTriangleCount; i++) {
            renderModel.ModifiableMesh().Triangles()[triangleIndex + i] += vertexIndex;
        }

        vertexIndex += modelVertexCount;
        triangleIndex += modelTriangleCount;
        uvIndex += modelUVCount;

        // 1 color per vertex
        colorIndex += modelVertexCount;
    }

    return renderModel;
}
