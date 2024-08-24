#include "ColorRenderer.h"
#include "GLShaderProgram.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

ColorRenderer::ColorRenderer(Color color, Vector2 worldSize) :
    model(worldSize),
    color(color) {
    model.SetMeshBuilderFunc([](MeshRendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    material = MakeMaterial(color);
}

ColorRenderer::ColorRenderer(SP<RenderMaterial> material, Color color, Vector2 worldSize) :
    model(worldSize),
    color(color) {
    this->material = material;

    model.SetMeshBuilderFunc([](MeshRendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });
}

SP<RenderMaterial> ColorRenderer::MakeMaterial(Color color) {
    auto program = SomeShaderProgram::registry.find("color.vary");
    GUARDR(program != SomeShaderProgram::registry.end(), nullptr)

    auto material = MAKE<RenderMaterial>();

    material->SetShaderProgram(program->second);

    bool isOpaque = color.IsOpaque();
    material->EnableFeature(RenderFeature::Blend, !isOpaque);

    return material;
}

VectorList<RenderModel> ColorRenderer::MakeRenderModels(RenderContextModel const& model) {
    VectorList<SomeTexture*> textures;
    VectorList<RenderModel> result;
    GUARDR(owner, result)

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    auto renderModel = builder.Build(
        this->model.Mesh(), *material, textures, ModelMatrix(),
        0 // TODO: rethink this: owner->transform.WorldPosition().z
    );

    VectorList<RenderColor> colors(this->model.Mesh().vertices.size(), color);
    renderModel.colors = colors;

    Add(result, renderModel);

    return result;
}
