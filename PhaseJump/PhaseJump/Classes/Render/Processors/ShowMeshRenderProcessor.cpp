#include "ShowMeshRenderProcessor.h"
#include "Colliders2D.h"
#include "Color.h"
#include "ColorRenderer.h"
#include "PolyFrameMeshBuilder.h"
#include "RenderWorldSystem.h"
#include "SomeCollider2D.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

void ShowMeshRenderProcessor::Process(RenderSystemModel& systemModel) {
    auto triMaterial = ColorRenderer::MakeMaterial(
        color.IsOpaque() ? RenderOpacityType::Opaque : RenderOpacityType::Blend
    );
    systemModel.materials.push_back(triMaterial);

    Mesh finalMesh;

    VectorList<RenderModel> renderModels;
    for (auto& renderModel : systemModel.models) {
        auto const& vertices = renderModel.Vertices();
        auto const& indices = renderModel.Indices();

        // Check: Size is: 3, i is 0, size - 2 is 1, 0 < 1, i + 2 is 2, which is a valid index
        for (size_t i = 0; (i + 2) < indices.size(); i += 3) {
            Polygon poly;

            poly.Add(vertices[indices[i]]);
            poly.Add(vertices[indices[i + 1]]);
            poly.Add(vertices[indices[i + 2]]);

            PolyFrameMeshBuilder builder(poly, 2);
            auto mesh = builder.BuildMesh();
            mesh *= renderModel.matrix;

            finalMesh += mesh;
        }
    }

    // FUTURE: we can optimize this by building only 1 render model
    // FUTURE: we can probably speed this up with a shader
    ColorRenderer renderer(triMaterial, color, vec2Zero);
    renderer.model.SetMeshBuilderFunc([&](RendererModel const& model) { return finalMesh; });

    auto thisRenderModels = renderer.MakeRenderModels();
    AddRange(renderModels, thisRenderModels);

    // Replace the existing models and only show the mesh
    systemModel.models = renderModels;
}
