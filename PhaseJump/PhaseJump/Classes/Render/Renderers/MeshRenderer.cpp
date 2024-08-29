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
    model(worldSize) {
    model.SetMeshBuilderFunc([](MeshRendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry["color.vary"];
    GUARD(program)

    material->SetShaderProgram(program);
}

VectorList<RenderModel> MeshRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures;
    auto renderModel = builder.Build(
        this->model.Mesh(), *material, textures, ModelMatrix(), 0
    ); // TODO: owner->transform.WorldPosition().z

    if (color) {
        renderModel.colors = VectorList<RenderColor>(this->model.Mesh().vertices.size(), *color);
    } else if (IsEmpty(renderModel.colors)) {
        renderModel.colors =
            VectorList<RenderColor>(this->model.Mesh().vertices.size(), Color::white);
    } else {
        renderModel.colors = colors;
    }

    Add(result, renderModel);
    return result;
}
