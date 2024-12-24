#include "SpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeShaderProgram.h"
#include "UIPlanner.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

SpriteRenderer::SpriteRenderer(Config config) :
    Base(Vector3{}) {

    if (config.texture) {
        auto texture = config.texture;

        model.material = MAKE<RenderMaterial>(RenderMaterial::Config{
            .shaderId = ShaderId::TextureVary, .features = config.features });
        model.material->Add(texture);
    } else if (config.material) {
        model.material = config.material;
    }

    if (model.material) {
        if (!IsEmpty(model.material->Textures())) {
            auto& texture = *model.material->Textures().begin();
            model.SetWorldSize(Vector2(texture->size.x, texture->size.y));
        }
    }

    model.SetBuildMeshFunc([this](RendererModel const& model) {
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

    PlanUIFunc planUIFunc = [](auto& component, String context, UIPlanner& planner) {
        auto renderer = static_cast<This*>(&component);

        planner
            .InputBool({ .label = "Flip X",
                         .binding = { [=]() { return renderer->flipX; },
                                      [=](auto& value) { renderer->SetFlipX(value); } } })
            .InputBool({ .label = "Flip Y",
                         .binding = { [=]() { return renderer->flipY; },
                                      [=](auto& value) { renderer->SetFlipY(value); } } });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

SpriteRenderer::SpriteRenderer(SP<SomeTexture> texture) :
    SpriteRenderer(Config{ .texture = texture }) {}

SpriteRenderer::SpriteRenderer(SP<RenderMaterial> material) :
    SpriteRenderer(Config{ .material = material }) {}

void SpriteRenderer::Configure() {
    model.SetBuildRenderModelsFunc([this](auto& model) {
        auto renderModel = RenderModelBuilder().Build(*this, model);
        GUARDR(renderModel, VectorList<RenderModel>())
        renderModel->SetVertexColors(std::span<RenderColor const>(model.VertexColors()));
        return VectorList<RenderModel>{ *renderModel };
    });
}
