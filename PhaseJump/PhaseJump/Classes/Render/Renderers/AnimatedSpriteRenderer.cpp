#include "AnimatedSpriteRenderer.h"
#include "QuadMeshBuilder.h"
#include "RateFramePlayable.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeShaderProgram.h"
#include "TextureAtlas.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

AnimatedSpriteRenderer::AnimatedSpriteRenderer(TextureList const& textures) :
    Base(vec2Zero) {
    std::transform(
        textures.cbegin(), textures.cend(), std::back_inserter(frames),
        [](SP<SomeTexture> texture) { return Frame(texture, vec2Zero); }
    );

    model.SetMeshBuilderFunc([](auto& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    model.material->SetShaderProgram(program->second);
    model.material->EnableFeature(RenderFeature::Blend, true);

    framePlayable = SCAST<SomeFramePlayable>(MAKE<RateFramePlayable>(textures.size(), 12.0f));

    if (!IsEmpty(textures)) {
        auto texture = textures[0];

        // This only works if we're using a texture atlas
        // FUTURE: support separate image animations if needed
        model.material->Add(texture);
        model.SetWorldSize(Vector2((float)texture->size.x, (float)texture->size.y));
    }
}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(TextureAtlas const& atlas) :
    Base(vec2Zero) {
    std::transform(
        atlas.Textures().cbegin(), atlas.Textures().cend(), std::back_inserter(frames),
        [](SP<AtlasTexture> texture) {
            Vector2 size(texture->size.x, texture->size.y);
            Vector2 trueSize(texture->trueSize.x, texture->trueSize.y);
            Vector2 trimOrigin(texture->trimOrigin.x, texture->trimOrigin.y);

            Vector2 truePosition(trueSize.x / 2.0f * vecLeft, trueSize.y / 2.0f * vecUp);
            Vector2 actualPosition(size.x / 2.0f * vecLeft, size.y / 2.0f * vecUp);
            Vector2 truePositionWithTrim = Vector2(
                truePosition.x + trimOrigin.x * vecRight, truePosition.y + trimOrigin.y * vecDown
            );

            return Frame(texture, truePositionWithTrim - actualPosition);
        }
    );

    model.material = MAKE<RenderMaterial>();

    model.SetMeshBuilderFunc([](auto& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    model.material->SetShaderProgram(program->second);
    model.material->EnableFeature(RenderFeature::Blend, true);

    auto const& textures = atlas.Textures();
    framePlayable = SCAST<SomeFramePlayable>(MAKE<RateFramePlayable>(textures.size(), 12.0f));

    if (!IsEmpty(textures)) {
        auto texture = textures[0];

        // This only works if we're using a texture atlas
        // FUTURE: support separate image animations if needed
        model.material->Add(texture);
        model.SetWorldSize(Vector2((float)texture->size.x, (float)texture->size.y));
    }
}

VectorList<RenderModel> AnimatedSpriteRenderer::MakeRenderModels() {
    VectorList<RenderModel> result;

    auto material = model.material;
    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    GUARDR(owner, result)
    GUARDR(frame >= 0 && frame < frames.size(), result)
    auto frameModel = frames[frame];

    auto& frameTexture = frameModel.texture;
    GUARDR(frameTexture, result)

    Mesh mesh = model.Mesh();
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
    auto renderModel = builder.Build(*this, mesh, *material, textures);

    renderModel.colors = VectorList<RenderColor>(mesh.Vertices().size(), color);

    result.push_back(renderModel);
    return result;
}

void AnimatedSpriteRenderer::SetColor(Color color) {
    this->color = color;
}

Vector2 AnimatedSpriteRenderer::Size() const {
    GUARDR(frame >= 0 && frame < frames.size(), vec2Zero)
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

    model.SetWorldSize(Size());
}
