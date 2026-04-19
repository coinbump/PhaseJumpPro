#include "SpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "ShaderProgram.h"
#include "SomeRenderEngine.h"
#include "StringUtils.h"
#include "UIPlanner.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

SpriteRenderer::SpriteRenderer(Config const& config) :
    core(this, Vector3{}) {

    if (config.texture) {
        auto texture = config.texture;

        core.model.material = MAKE<RenderMaterial>(RenderMaterial::Config{
            .shaderId = ShaderId::TextureVary, .features = config.features });
        core.model.material->Add(texture);
    } else if (config.material) {
        core.model.material = config.material;
    }

    if (core.model.material) {
        if (!IsEmpty(core.model.material->Textures())) {
            auto& texture = *core.model.material->Textures().begin();
            core.model.SetWorldSize(Vector2(texture->size.x, texture->size.y));
        }
    }

    core.model.SetBuildMeshFunc([this](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        auto result = builder.BuildMesh();

        if (flipX || flipY) {
            std::transform(
                result.UVs().cbegin(), result.UVs().cend(), result.UVs().begin(),
                [this](Vector2 uv) {
                    if (flipX) {
                        uv.x = 1.0f - uv.x;
                    }

                    if (flipY) {
                        uv.y = 1.0f - uv.y;
                    }
                    return uv;
                }
            );
        }

        GUARDR(model.Material(), result)
        auto& textures = model.Material()->Textures();
        if (!IsEmpty(textures)) {
            UVTransformFuncs::textureCoordinates(*textures[0], result.UVs());
        }

        return result;
    });
    Configure();

    Override(planUIFuncs[UIContextId::Inspector], core.MakePlanUIFunc());

    PlanUIFunc planUIFunc = [this](auto args) {
        args.planner.LabelText([this]() {
            auto size = core.model.WorldSize();
            return UIPlanner::LabelTextConfig{ .label = "World Size",
                                               .text = std::format("{}, {}", size.x, size.y) };
        });

        args.planner.LabelText([this]() {
            auto material = core.model.Material();
            if (material) {
                auto texture = core.model.Material()->Textures()[0];
                if (texture) {
                    return UIPlanner::LabelTextConfig{ .label = "Texture ID",
                                                       .text = MakeString(texture->RenderId()) };
                }
            }

            return UIPlanner::LabelTextConfig{ .label = "Texture ID: MISSING" };
        });

        args.planner
            .InputBool({ .label = "Flip X",
                         .binding = Binding<bool>({ .getFunc = [this]() { return flipX; },
                                                    .setFunc = [this](auto& value
                                                               ) { SetFlipX(value); } }) })
            .InputBool({ .label = "Flip Y",
                         .binding = Binding<bool>({ .getFunc = [this]() { return flipY; },
                                                    .setFunc = [this](auto& value
                                                               ) { SetFlipY(value); } }) });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

SpriteRenderer::SpriteRenderer(SP<Texture> texture) :
    SpriteRenderer(Config{ .texture = texture }) {}

SpriteRenderer::SpriteRenderer(SP<RenderMaterial> material) :
    SpriteRenderer(Config{ .material = material }) {}

void SpriteRenderer::Configure() {
    core.model.SetBuildRenderModelsFunc([this](auto& model) {
        auto renderModel = RenderModelBuilder().Build(core, model);
        GUARDR(renderModel, VectorList<RenderModel>())
        renderModel->SetVertexColors(std::span<RenderColor const>(model.VertexColors()));
        return VectorList<RenderModel>{ *renderModel };
    });
}
