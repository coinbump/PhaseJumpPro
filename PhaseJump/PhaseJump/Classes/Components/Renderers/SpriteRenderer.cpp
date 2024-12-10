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
            .shaderId = "texture.vary", .features = config.features });
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

    model.SetBuildMeshFunc([](RendererModel const& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });
    Configure();

    PlanUIFunc planUIFunc = [](auto& component, String context, UIPlanner& planner) {
        auto renderer = static_cast<This*>(&component);

        planner
            .InputBool(
                "Flip X", { [=]() { return renderer->flipX; },
                            [=](auto& value) { renderer->SetFlipX(value); } }
            )
            .InputBool(
                "Flip Y", { [=]() { return renderer->flipY; },
                            [=](auto& value) { renderer->SetFlipY(value); } }
            );
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

SpriteRenderer::SpriteRenderer(SP<SomeTexture> texture) :
    SpriteRenderer(Config{ .texture = texture }) {}

SpriteRenderer::SpriteRenderer(SP<RenderMaterial> material) :
    SpriteRenderer(Config{ .material = material }) {}

void SpriteRenderer::Configure() {
    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<RenderModel> result;

        auto material = model.material;
        if (nullptr == material) {
            PJ::Log("ERROR. Missing material.");
            return result;
        }

        Mesh mesh = model.Mesh();

        if (flipX || flipY) {
            std::transform(
                mesh.UVs().cbegin(), mesh.UVs().cend(), mesh.UVs().begin(),
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

        RenderModelBuilder builder;
        auto renderModel = builder.Build(*this, mesh, *material, material->Textures());
        renderModel.SetVertexColors(std::span<RenderColor const>(model.VertexColors()));

        result.push_back(renderModel);
        return result;
    });
}
