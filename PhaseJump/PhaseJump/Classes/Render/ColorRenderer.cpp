#include "ColorRenderer.h"
#include "GLShaderProgram.h"
#include "QuadMeshBuilder.h"
#include "RenderFeature.h"
#include "RenderIntoModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

ColorRenderer::ColorRenderer(RGBAColor color, Vector2 worldSize) :
    model(worldSize) {
    material = MAKE<RenderMaterial>();

    model.SetMeshBuilderFunc([](MeshRendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    auto program = GLShaderProgram::registry.find("color.uniform");
    GUARD(program != GLShaderProgram::registry.end())

    material->SetShaderProgram(program->second);
    material->AddUniformColor(color);

    bool isOpaque = color.IsOpaque();
    material->EnableFeature(RenderFeature::Blend, !isOpaque);
}

VectorList<RenderModel> ColorRenderer::MakeRenderModels(RenderIntoModel const& model) {
    VectorList<SomeTexture*> textures;
    return Base::MakeRenderModels(model, this->model.Mesh(), textures);
}
