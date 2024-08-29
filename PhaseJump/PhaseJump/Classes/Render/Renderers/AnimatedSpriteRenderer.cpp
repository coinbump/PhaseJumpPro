#include "AnimatedSpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RateFramePlayable.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeShaderProgram.h"
#include "TextureAtlas.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

AnimatedSpriteRenderer::AnimatedSpriteRenderer(TextureList const& textures) {
    std::transform(
        textures.cbegin(), textures.cend(), std::back_inserter(frames),
        [](SP<SomeTexture> texture) { return Frame(texture, Vector2::zero); }
    );

    material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    material->SetShaderProgram(program->second);

    framePlayable = SCAST<SomeFramePlayable>(MAKE<RateFramePlayable>(textures.size(), 12.0f));

    if (!IsEmpty(textures)) {
        auto texture = textures[0];
        material->Add(texture);

        QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
        mesh = builder.BuildMesh();
    }
}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(TextureAtlas const& atlas) {
    std::transform(
        atlas.Textures().cbegin(), atlas.Textures().cend(), std::back_inserter(frames),
        [](SP<AtlasTexture> texture) {
            Vector2 size(texture->size.x, texture->size.y);
            Vector2 trueSize(texture->trueSize.x, texture->trueSize.y);
            Vector2 trimOrigin(texture->trimOrigin.x, texture->trimOrigin.y);

            Vector2 truePosition(
                trueSize.x / 2.0f * Vector2::left.x, trueSize.y / 2.0f * Vector2::up.y
            );
            Vector2 actualPosition(size.x / 2.0f * Vector2::left.x, size.y / 2.0f * Vector2::up.y);
            Vector2 truePositionWithTrim = Vector2(
                truePosition.x + trimOrigin.x * Vector2::right.x,
                truePosition.y + trimOrigin.y * Vector2::down.y
            );

            return Frame(texture, truePositionWithTrim - actualPosition);
        }
    );

    material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    material->SetShaderProgram(program->second);

    auto const& textures = atlas.Textures();
    framePlayable = SCAST<SomeFramePlayable>(MAKE<RateFramePlayable>(textures.size(), 12.0f));

    if (!IsEmpty(textures)) {
        auto texture = textures[0];
        material->Add(texture);

        QuadMeshBuilder builder(Vector2((float)texture->size.x, (float)texture->size.y));
        mesh = builder.BuildMesh();
    }
}

VectorList<RenderModel> AnimatedSpriteRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    GUARDR(owner, result)
    GUARDR(frame >= 0 && frame < frames.size(), result)
    auto frameModel = frames[frame];

    auto& frameTexture = frameModel.texture;
    GUARDR(frameTexture, result)

    Mesh mesh = this->mesh;
    auto offset = frameModel.offset;
    if (flipX) {
        offset.x = -offset.x;
    }
    if (flipY) {
        offset.y = -offset.y;
    }

    mesh.OffsetBy(offset);

    std::transform(mesh.UVs().cbegin(), mesh.UVs().cend(), mesh.UVs().begin(), [this](Vector2 uv) {
        if (flipX) {
            uv.x = 1.0f - uv.x;
        }

        if (flipY) {
            uv.y = 1.0f - uv.y;
        }
        return uv;
    });

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures{ frameModel.texture.get() };
    auto renderModel =
        builder.Build(mesh, *material, textures, ModelMatrix(), owner->transform.WorldPosition().z);

    renderModel.colors = VectorList<RenderColor>(mesh.vertices.size(), color);

    result.push_back(renderModel);
    return result;
}

void AnimatedSpriteRenderer::SetColor(Color color) {
    this->color = color;
}

Vector2 AnimatedSpriteRenderer::Size() const {
    GUARDR(frame >= 0 && frame < frames.size(), Vector2::zero)
    auto const& texture = frames[frame].texture;
    auto textureSize = texture->Size();

    return Vector2(textureSize.x, textureSize.y);
}

void AnimatedSpriteRenderer::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    if (IsEmpty(frames)) {
        return;
    }
    GUARD(framePlayable)

    int oldFrame = frame;
    framePlayable->OnUpdate(time);
    frame = framePlayable->Frame();

    if (oldFrame == frame) {
        return;
    }

    GUARD(frame >= 0 && frame < frames.size())
    auto frameModel = frames[frame];

    QuadMeshBuilder builder(
        Vector2((float)frameModel.texture->size.x, (float)frameModel.texture->size.y)
    );
    mesh = builder.BuildMesh();
}
