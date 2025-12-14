#include "SimpleGradientRenderer.h"
#include "GLShaderProgram.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

SimpleGradientRenderer::SimpleGradientRenderer(Config config) :
    Base(config.worldSize),
    startColor(config.startColor),
    endColor(config.endColor) {
    model.material =
        MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::ColorVary });

    model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.SetBuildVertexColorsFunc([this](auto& model, auto& colors) {
        colors.clear();
        Add(colors, startColor);
        Add(colors, endColor);
        Add(colors, startColor);
        Add(colors, endColor);
    });

    // FUTURE: support different gradient directions if needed
    // FUTURE: support MultiGradientRenderer with multiple color stops if needed

    bool isOpaque = startColor.IsOpaque() && endColor.IsOpaque();
    model.material->EnableFeature(RenderFeature::Blend, !isOpaque);
}
