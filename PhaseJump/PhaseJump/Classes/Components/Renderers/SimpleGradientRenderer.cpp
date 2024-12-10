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
    Base(worldSize),
    startColor(startColor),
    endColor(endColor) {
    model.material = MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = "color.vary" });

    model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.SetBuildVertexColorsFunc([=](auto& model, auto& colors) {
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
