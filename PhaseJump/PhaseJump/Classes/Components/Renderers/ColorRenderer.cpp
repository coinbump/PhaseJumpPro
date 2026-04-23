#include "ColorRenderer.h"
#include "GLShaderProgram.h"
#include "MaterialRenderModel.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "SomeRenderEngine.h"
#include "UIPlanner.h"

using namespace std;
using namespace PJ;

ColorRenderer::ColorRenderer(Config const& config) :
    core(this, config.worldSize) {

    core.model.material = config.material ? config.material : MakeMaterial(config.opacityType);
    core.model.SetColor(config.color);

    core.model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    Override(planUIFuncs[UIContextId::Inspector], core.MakePlanUIFunc());
}

SP<RenderMaterial> ColorRenderer::MakeMaterial(RenderOpacityType opacityType) {
    auto material = MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::ColorVary });
    material->EnableFeature(RenderFeature::Blend, opacityType == RenderOpacityType::Blend);

    return material;
}

void ColorRenderer::EnableBlend(bool isFeatureEnabled) {
    auto material = core.model.Material();
    GUARD(material)
    material->EnableFeature(RenderFeature::Blend, isFeatureEnabled);
}
