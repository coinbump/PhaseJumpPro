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
    Base(worldSize),
    color(color) {
    model.material =
        MakeMaterial(color.IsOpaque() ? RenderOpacityType::Opaque : RenderOpacityType::Blend);

    model.SetMeshBuilderFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });
}

ColorRenderer::ColorRenderer(SP<RenderMaterial> material, Color color, Vector2 worldSize) :
    Base(worldSize),
    color(color) {
    model.material = material;

    model.SetMeshBuilderFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });
}

SP<RenderMaterial> ColorRenderer::MakeMaterial(RenderOpacityType opacityType) {
    auto program = SomeShaderProgram::registry.find("color.vary");
    GUARDR(program != SomeShaderProgram::registry.end(), nullptr)

    auto material = MAKE<RenderMaterial>();

    material->SetShaderProgram(program->second);

    material->EnableFeature(RenderFeature::Blend, opacityType == RenderOpacityType::Blend);

    return material;
}

VectorList<RenderModel> ColorRenderer::MakeRenderModels() {
    VectorList<SomeTexture*> textures;
    VectorList<RenderModel> result;

    auto material = model.material;
    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    auto renderModel = builder.Build(*this, model.Mesh(), *material, textures);

    VectorList<RenderColor> colors(model.Mesh().Vertices().size(), color);
    renderModel.colors = colors;

    Add(result, renderModel);

    return result;
}
