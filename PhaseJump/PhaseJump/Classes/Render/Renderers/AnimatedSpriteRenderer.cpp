#include "AnimatedSpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RenderMaterial.h"
#include "RenderIntoModel.h"
#include "SomeRenderEngine.h"
#include "RenderModelBuilder.h"
#include "RenderModel.h"
#include "Macros.h"
#include "SomeFrameAnimator.h"

using namespace std;
using namespace PJ;

AnimatedSpriteRenderer::AnimatedSpriteRenderer(TextureList const& textures) : textures(textures) {
    material = MAKE<RenderMaterial>();
    
    frameAnimator = SCAST<SomeFrameAnimator>(MAKE<RateFrameAnimator>(5.0f));

    if (!textures.IsEmpty()) {
        auto texture = textures[0];
        material->textures.Add(textures[0]);

        QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
        mesh = builder.BuildMesh();
    }
}

void AnimatedSpriteRenderer::RenderInto(RenderIntoModel model) {
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

void AnimatedSpriteRenderer::SetColor(Color color) {
    if (material) {
        if (material->uniformColors.IsEmpty()) {
            material->uniformColors.Add(color);
        } else {
            material->uniformColors[0] = color;
        }
    }
}

void AnimatedSpriteRenderer::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    if (textures.IsEmpty()) {
        return;
    }
    if (nullptr == frameAnimator) {
        return;
    }

    int oldFrame = frame;
    frame = frameAnimator->FrameForUpdate(time, int(textures.size()));

    if (oldFrame == frame) {
        return;
    }

    auto texture = textures[frame];
    material->textures = VectorList<SP<SomeTexture>>{texture};

    QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
    mesh = builder.BuildMesh();
}
