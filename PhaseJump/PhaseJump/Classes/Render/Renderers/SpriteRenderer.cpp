#include "SpriteRenderer.h"
#include "QuadRenderMeshBuilder.h"
#include "RenderMaterial.h"
#include "RenderIntoModel.h"
#include "SomeRenderEngine.h"
#include "RenderModelBuilder.h"
#include "RenderModel.h"
#include "Macros.h"

using namespace std;
using namespace PJ;

SpriteRenderer::SpriteRenderer(SP<SomeTexture> texture) {
    material = MAKE<RenderMaterial>();
    material->textures.Add(texture);

    QuadRenderMeshBuilder builder(Vector2(texture->size.x, texture->size.y));
    mesh = builder.BuildRenderMesh();
}

void SpriteRenderer::RenderInto(RenderIntoModel model) {
    if (nullptr == material || nullptr == material->shaderProgram) {
        PJLog("ERROR. Missing material.");
        return;
    }

    RenderModelBuilder builder;
    auto renderModel = builder.Build(*material->shaderProgram,
                                     mesh,
                                     *material,
                                     model.modelMatrix,
                                     owner.lock()->transform->WorldPosition().z);

    std::transform(renderModel.uvs.begin(), renderModel.uvs.end(), renderModel.uvs.begin(), [this] (Vector2 uv) {
        if (flipX) {
            uv.x = 1.0f - uv.x;
        }

        if (flipY) {
            uv.y = 1.0f - uv.y;
        }
        return uv;
    });

    model.renderContext->renderEngine->RenderProcess(renderModel);
}

Vector2 SpriteRenderer::Size() const {
    if (material && !material->textures.IsEmpty()) {
        auto texture = material->textures[0];
        if (texture) {
            return Vector2(texture->size.x, texture->size.y);
        }
    }

    return Vector2();
}

void SpriteRenderer::SetColor(Color color) {
    if (material) {
        if (material->uniformColors.IsEmpty()) {
            material->uniformColors.Add(color);
        } else {
            material->uniformColors[0] = color;
        }
    }
}
