#include "SimpleGradientRenderer.h"
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

SimpleGradientRenderer::SimpleGradientRenderer(
    Color startColor, Color endColor, Vector2 worldSize
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
    auto program = SomeShaderProgram::registry.find("color.vary");
    GUARD(program != SomeShaderProgram::registry.end())

    // FUTURE: support a colors builder that works for different mesh types
    material->SetShaderProgram(program->second);

    bool isOpaque = startColor.IsOpaque() && endColor.IsOpaque();
    material->EnableFeature(RenderFeature::Blend, !isOpaque);
}

VectorList<RenderModel> SimpleGradientRenderer::MakeRenderModels() {
    VectorList<SomeTexture*> textures;

    auto result = this->Base::MakeRenderModels(this->model.Mesh(), textures);

    if (!IsEmpty(result)) {
        Add(result[0].colors, startColor);
        Add(result[0].colors, endColor);
        Add(result[0].colors, startColor);
        Add(result[0].colors, endColor);
    }

    return result;
}
