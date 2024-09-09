#include "MeshRenderer.h"
#include "Angle.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeShaderProgram.h"

using namespace std;
using namespace PJ;

MeshRenderer::MeshRenderer(Vector3 worldSize) :
    Base(worldSize) {
    model.SetMeshBuilderFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry["color.vary"];
    GUARD(program)

    model.material->SetShaderProgram(program);
}

VectorList<RenderModel> MeshRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

    auto material = model.material;
    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures;
    auto renderModel = builder.Build(*this, model.Mesh(), *material, textures);

    if (color) {
        renderModel.colors = VectorList<RenderColor>(model.Mesh().Vertices().size(), *color);
    } else if (IsEmpty(renderModel.colors)) {
        renderModel.colors =
            VectorList<RenderColor>(this->model.Mesh().Vertices().size(), Color::white);
    } else {
        // TODO: mod fill the colors so they match the vertices size
        renderModel.colors = colors;
    }

    Add(result, renderModel);
    return result;
}
