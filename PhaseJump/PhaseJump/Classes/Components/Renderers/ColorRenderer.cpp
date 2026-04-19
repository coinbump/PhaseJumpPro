#include "ColorRenderer.h"
#include "GLShaderProgram.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "SomeRenderEngine.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

ColorRenderer::ColorRenderer(Config const& config) :
    core(this, config.worldSize) {

    core.model.material =
        config.material ? config.material : MakeMaterial(RenderOpacityTypeFor(config.color));
    core.model.SetColor(config.color);

    core.model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    core.model.SetOnColorsChangeFunc([](auto& model) {
        auto material = model.material;
        GUARD(material)

        auto& colors = model.Colors();
        GUARD(!IsEmpty(colors))

        auto color = colors[0];
        material->EnableFeature(RenderFeature::Blend, color.a != 1);
    });

    Override(planUIFuncs[UIContextId::Inspector], core.MakePlanUIFunc());
}

SP<RenderMaterial> ColorRenderer::MakeMaterial(RenderOpacityType opacityType) {
    auto material = MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::ColorVary });
    material->EnableFeature(RenderFeature::Blend, opacityType == RenderOpacityType::Blend);

    return material;
}

void ColorRenderer::EnableBlend(bool isFeatureEnabled) {
    core.model.SetOnColorsChangeFunc({});

    auto material = core.model.Material();
    GUARD(material)
    material->EnableFeature(RenderFeature::Blend, isFeatureEnabled);
}
