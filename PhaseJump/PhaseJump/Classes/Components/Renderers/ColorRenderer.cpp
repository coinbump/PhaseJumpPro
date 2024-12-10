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

ColorRenderer::ColorRenderer(Config config) :
    Base(config.worldSize) {

    model.material =
        config.material ? config.material : MakeMaterial(RenderOpacityTypeFor(config.color));
    model.SetColor(config.color);

    model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.SetOnColorsChangeFunc([](auto& model) {
        auto material = model.material;
        GUARD(material)

        auto& colors = model.Colors();
        GUARD(!IsEmpty(colors))

        auto color = colors[0];
        material->EnableFeature(RenderFeature::Blend, color.a != 1);
    });
}

ColorRenderer::ColorRenderer(Color color, Vector2 worldSize) :
    ColorRenderer({ .color = color, .worldSize = worldSize }) {}

SP<RenderMaterial> ColorRenderer::MakeMaterial(RenderOpacityType opacityType) {
    auto material = MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = "color.vary" });
    material->EnableFeature(RenderFeature::Blend, opacityType == RenderOpacityType::Blend);

    return material;
}

void ColorRenderer::EnableBlend(bool isFeatureEnabled) {
    model.SetOnColorsChangeFunc({});

    auto material = model.Material();
    GUARD(material)
    material->EnableFeature(RenderFeature::Blend, isFeatureEnabled);
}
