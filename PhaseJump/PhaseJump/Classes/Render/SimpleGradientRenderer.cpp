#include "SimpleGradientRenderer.h"
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

SimpleGradientRenderer::SimpleGradientRenderer(
    RGBAColor startColor, RGBAColor endColor, Vector2 worldSize
) :
    model(worldSize),
    startColor(startColor),
    endColor(endColor) {
    material = MAKE<RenderMaterial>();

    model.SetMeshBuilderFunc([](MeshRendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    // FUTURE: support different gradient directions if needed
    // FUTURE: support MultiGradientRenderer with multiple color stops if needed
    auto program = GLShaderProgram::registry.find("color.vary");
    GUARD(program != GLShaderProgram::registry.end())

    // FUTURE: support a colors builder that works for different mesh types
    material->SetShaderProgram(program->second);

    bool isOpaque = startColor.IsOpaque() && endColor.IsOpaque();
    material->EnableFeature(RenderFeature::Blend, !isOpaque);
}

VectorList<RenderModel> SimpleGradientRenderer::MakeRenderModels(RenderIntoModel const& model) {
    VectorList<SomeTexture*> textures;
    auto result = Base::MakeRenderModels(model, this->model.Mesh(), textures);

    if (!IsEmpty(result)) {
        result[0].colors.Add(startColor);
        result[0].colors.Add(endColor);
        result[0].colors.Add(startColor);
        result[0].colors.Add(endColor);
    }

    return result;
}
