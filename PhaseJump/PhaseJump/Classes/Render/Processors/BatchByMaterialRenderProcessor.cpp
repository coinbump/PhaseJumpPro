#include "BatchByMaterialRenderProcessor.h"
#include "RenderWorldSystem.h"

using namespace std;
using namespace PJ;

// TODO: needs unit tests
void BatchByMaterialRenderProcessor::Process(RenderCameraModel& cameraModel) {
    // Force z order based on model order (used by opaque objects to optimize renders by using the
    // depth buffer)
    float z = 0;
    float zStep = 1.0f / cameraModel.renderModels.size();

    {
#ifdef PROFILE
        DevProfiler devProfiler("Camera- Combine RenderModels", [](String value) {
            std::cout << value;
        });
#endif
        // Don't make extra copies if there's nothing to batch
        GUARD(cameraModel.renderModels.size() > 1)

        VectorList<RenderModel> models;

        VectorList<RenderModel*> cameraModels;
        std::transform(
            cameraModel.renderModels.begin(), cameraModel.renderModels.end(),
            std::back_inserter(cameraModels), [](auto& model) { return &model; }
        );

        // Opaque objects are rendered behind blended render models
        VectorList<RenderModel*> noBlendRenderModels;
        std::copy_if(
            cameraModels.begin(), cameraModels.end(), std::back_inserter(noBlendRenderModels),
            [](auto& model) { return !model->IsFeatureEnabled(RenderFeature::Blend); }
        );

        std::for_each(
            noBlendRenderModels.begin(), noBlendRenderModels.end(),
            [&](RenderModel* model) {
                model->z = z;
                z += zStep * Vector3::back.z;
            }
        );

        // Transparent objects must be above opaque objects
        // If you want objects to respect Z-ordering, they must have the same blend type
        VectorList<RenderModel*> blendRenderModels;
        std::copy_if(
            cameraModels.begin(), cameraModels.end(), std::back_inserter(blendRenderModels),
            [](auto& model) { return model->IsFeatureEnabled(RenderFeature::Blend); }
        );

        // Process a list of render models
        auto processModels = [&](VectorList<RenderModel*>& cameraModels) {
            GUARD(!IsEmpty(cameraModels))

            String prevPropertyId;
            std::stack<RenderModel*> modelStack;

            bool isBlended = cameraModels[0]->IsFeatureEnabled(RenderFeature::Blend);

            // Combine a list of render models
            auto combineRecent = [&]() {
                VectorList<RenderModel*> subModels;

                while (!IsEmpty(modelStack)) {
                    auto top = modelStack.top();
                    auto topPropertyId = top->Material()->PropertyId();

                    if (topPropertyId == prevPropertyId) {
                        subModels.push_back(top);
                        modelStack.pop();
                    } else {
                        break;
                    }
                }

                // Blended were inserted in the wrong order, so reverse them
                if (isBlended) {
                    std::reverse(subModels.begin(), subModels.end());
                }

                auto combinedSubModel = Combine(subModels);
                if (combinedSubModel) {
                    models.push_back(*combinedSubModel);
                };
            };

            for (auto& renderModel : cameraModels) {
                auto modelMaterial = renderModel->Material();
                GUARD_CONTINUE(modelMaterial)

                String propertyId = modelMaterial->PropertyId();
                if (prevPropertyId == propertyId || IsEmpty(modelStack)) {
                    modelStack.push(renderModel);
                } else {
                    combineRecent();
                    modelStack.push(renderModel);
                }
                prevPropertyId = propertyId;
            }
            combineRecent();
        };
        processModels(noBlendRenderModels);
        processModels(blendRenderModels);

        cameraModel.renderModels = models;
    }
}

std::optional<RenderModel>
BatchByMaterialRenderProcessor::Combine(VectorList<RenderModel*>& renderModels) {
    GUARDR(!IsEmpty(renderModels), {})

    if (!renderModels[0]->IsFeatureEnabled(RenderFeature::Blend)) {
        for (auto& modelP : renderModels) {
            auto& model = *modelP;
            // Force the z order of opaque objects for depth buffer optimization
            std::for_each(
                model.mesh.Vertices().begin(), model.mesh.Vertices().end(),
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
        model->mesh *= model->matrix;
        model->matrix.LoadIdentity();
    }

    size_t vertexCount = 0;
    size_t trianglesCount = 0;
    size_t uvsCount = 0;
    size_t colorsCount = 0;

    for (auto& modelP : renderModels) {
        auto& model = *modelP;
        vertexCount += model.mesh.Vertices().size();
        trianglesCount += model.mesh.Triangles().size();
        uvsCount += model.mesh.UVs().size();

        // 1 color per vertex
        colorsCount += model.mesh.Vertices().size();

        auto thisColorsCount = model.VertexColors().size();
        auto thisVertexCount = model.mesh.Vertices().size();
        if (thisColorsCount != thisVertexCount) {
            PJ::Log(
                "WARNING: colorsCount ", (int)thisColorsCount, " and vertexCount ",
                (int)thisVertexCount, " don't match"
            );
        }
    }

    RenderModel renderModel = *renderModels[0];
    VectorList<Vector3> vertices;
    vertices.resize(vertexCount);
    renderModel.mesh.SetVertices(vertices);
    renderModel.mesh.Triangles().resize(trianglesCount);

    VectorList<Vector2> uvs;
    uvs.resize(uvsCount);
    renderModel.mesh.SetUVs(uvs);
    renderModel.ModifiableVertexColors().resize(colorsCount);

    size_t vertexIndex = 0;
    size_t triangleIndex = 0;
    size_t uvIndex = 0;
    size_t colorIndex = 0;

    for (auto& modelP : renderModels) {
        auto& model = *modelP;
        auto modelVertexCount = model.mesh.Vertices().size();
        auto modelTriangleCount = model.mesh.Triangles().size();
        auto modelUVCount = model.mesh.UVs().size();
        auto modelColorCount = model.VertexColors().size();

        std::copy_n(
            model.mesh.Vertices().cbegin(), modelVertexCount,
            renderModel.mesh.Vertices().begin() + vertexIndex
        );
        std::copy_n(
            model.mesh.UVs().cbegin(), modelUVCount, renderModel.mesh.UVs().begin() + uvIndex
        );
        std::copy_n(
            model.mesh.Triangles().begin(), modelTriangleCount,
            renderModel.mesh.Triangles().begin() + triangleIndex
        );

        // Max: 1 color per vertex
        std::copy_n(
            model.VertexColors().begin(), std::min(modelColorCount, modelVertexCount),
            renderModel.ModifiableVertexColors().begin() + colorIndex
        );

        for (int i = 0; i < modelTriangleCount; i++) {
            renderModel.mesh.Triangles()[triangleIndex + i] += vertexIndex;
        }

        vertexIndex += modelVertexCount;
        triangleIndex += modelTriangleCount;
        uvIndex += modelUVCount;

        // 1 color per vertex
        colorIndex += modelVertexCount;
    }

    return renderModel;
}
