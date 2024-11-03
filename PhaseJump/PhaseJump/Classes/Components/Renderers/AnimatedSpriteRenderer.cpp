#include "AnimatedSpriteRenderer.h"
#include "CollectionUtils.h"
#include "FramePlayable.h"
#include "Funcs.h"
#include "QuadMeshBuilder.h"
#include "RenderContextModel.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "SomeShaderProgram.h"
#include "TextureAtlas.h"
#include "UIPlanner.h"
#include "Utils.h"
#include "World.h"

using namespace std;
using namespace PJ;

UnorderedSet<SP<SomeTexture>>
FindTextures(World& world, VectorList<AnimatedSpriteRenderer::KeyframeModel> const& models) {
    VectorList<String> textureIds =
        Map<String>(models, [](auto& model) { return model.textureId; });
    textureIds = Filter(textureIds, [](auto& textureId) { return textureId.length() > 0; });

    UnorderedSet<String> textureIdSet(textureIds.begin(), textureIds.end());
    return world.FindTextures(textureIdSet);
}

void AnimatedSpriteRenderer::SetTextures(VectorList<SP<SomeTexture>> const& textures) {
    frames.clear();

    std::transform(
        textures.cbegin(), textures.cend(), std::back_inserter(frames),
        [](SP<SomeTexture> texture) {
            Vector2 size(texture->size.x, texture->size.y);
            Vector2 untrimmedSize(texture->UntrimmedSize().x, texture->UntrimmedSize().y);
            Vector2 trimOrigin(texture->TrimOrigin().x, texture->TrimOrigin().y);

            Vector2 truePosition(untrimmedSize.x / 2.0f * vecLeft, untrimmedSize.y / 2.0f * vecUp);
            Vector2 actualPosition(size.x / 2.0f * vecLeft, size.y / 2.0f * vecUp);
            Vector2 truePositionWithTrim = Vector2(
                truePosition.x + trimOrigin.x * vecRight, truePosition.y + trimOrigin.y * vecDown
            );

            return Frame(texture, truePositionWithTrim - actualPosition);
        }
    );
}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(
    Vector3 worldSize, float frameRate, AnimationCycleType cycleType
) :
    Base(worldSize) {

    framePlayable = NEW<FramePlayable>(0, frameRate, cycleType);

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

        FramePlayable* rateFramePlayable =
            dynamic_cast<FramePlayable*>(renderer->framePlayable.get());
        if (rateFramePlayable) {
            VectorList<String> cycleOptions{ "Once", "PingPong", "Loop" };
            planner
                .InputFloat(
                    "Frame rate", { [=]() { return rateFramePlayable->FrameRate(); },
                                    [=](auto& value) { rateFramePlayable->SetFrameRate(value); } }
                )
                .PickerList(
                    "Animation cycle", cycleOptions,
                    { [=]() { return (int)rateFramePlayable->CycleType(); },
                      [=](auto& value) {
                          rateFramePlayable->SetCycleType((AnimationCycleType)value);
                      } }
                );
        }
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);

    Configure();
}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(
    TextureList const& textures, float frameRate, AnimationCycleType cycleType
) :
    AnimatedSpriteRenderer(vec2Zero, frameRate, cycleType) {
    SetTextures(textures);

    model.SetBuildMeshFunc([](auto& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    model.material = MAKE<RenderMaterial>();

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    model.material->SetShaderProgram(program->second);
    model.material->EnableFeature(RenderFeature::Blend, true);

    framePlayable = NEW<FramePlayable>(textures.size(), frameRate, cycleType);
    SetCycleType(cycleType);

    // Synchronize states
    OnFrameChange();
}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(
    TextureAtlas const& atlas, float frameRate, AnimationCycleType cycleType
) :
    AnimatedSpriteRenderer(vec2Zero, frameRate, cycleType) {
    VectorList<SP<SomeTexture>> someTextures(atlas.Textures().begin(), atlas.Textures().end());
    SetTextures(someTextures);

    model.material = MAKE<RenderMaterial>();

    model.SetBuildMeshFunc([](auto& model) {
        QuadMeshBuilder builder(model.WorldSize());
        return builder.BuildMesh();
    });

    auto program = SomeShaderProgram::registry.find("texture.vary");
    GUARD(program != SomeShaderProgram::registry.end())
    model.material->SetShaderProgram(program->second);
    model.material->EnableFeature(RenderFeature::Blend, true);

    auto const& textures = atlas.Textures();
    framePlayable = NEW<FramePlayable>(textures.size(), frameRate, cycleType);
    SetCycleType(cycleType);

    // Synchronize states
    OnFrameChange();
}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(
    VectorList<KeyframeModel> const& models, float frameRate, AnimationCycleType cycleType
) :
    AnimatedSpriteRenderer(TextureList{}, frameRate, cycleType) {

    // Keyframes require textures, so wait until Awake when we have a world
    keyframeModels = models;
}

void AnimatedSpriteRenderer::Awake() {
    if (!IsEmpty(keyframeModels)) {
        SetFrames(keyframeModels);
        keyframeModels.clear();
    }
}

void AnimatedSpriteRenderer::Configure() {
    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<RenderModel> result;

        auto material = model.material;
        if (nullptr == material) {
            PJ::Log("ERROR. Missing material.");
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

        RenderModelBuilder builder;
        VectorList<SP<SomeTexture>> textures{ frameModel.texture };
        auto renderModel = builder.Build(*this, mesh, *material, textures);
        renderModel.SetVertexColors(std::span<RenderColor const>(model.VertexColors()));

        result.push_back(renderModel);
        return result;
    });
}

void AnimatedSpriteRenderer::Reset() {
    Base::Reset();

    SetFrame(0);
}

Vector2 AnimatedSpriteRenderer::Size() const {
    GUARDR(frame >= 0 && frame < frames.size(), vec2Zero)
    auto const& texture = frames[frame].texture;
    auto textureSize = texture->Size();

    return Vector2(textureSize.x, textureSize.y);
}

void AnimatedSpriteRenderer::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARD(!IsEmpty(frames))
    GUARD(framePlayable)

    framePlayable->OnUpdate(time);
    auto frame = framePlayable->Frame();

    SetFrame(frame);
}

void AnimatedSpriteRenderer::SetFrame(int value) {
    GUARD(frame != value)
    GUARD(value >= 0 && value < frames.size())

    frame = value;
    OnFrameChange();
}

void AnimatedSpriteRenderer::OnFrameChange() {
    GUARD(frame >= 0 && frame < frames.size())
    auto frameModel = frames[frame];

    auto& frameTexture = frameModel.texture;
    GUARD(frameTexture)

    model.material->SetTexture(frameTexture);
    model.SetWorldSize(Size());
    model.SetRenderModelsNeedBuild();

    PJ::LogIf(_diagnose, "Frame Change: ", MakeString(frame), " texture: ", frameTexture->id);
}

void AnimatedSpriteRenderer::SetFrames(VectorList<KeyframeModel> models) {
    GUARD_LOG(owner && owner->World(), "ERROR. Need world for SetFrames to find textures")

    auto frameRate = framePlayable->FrameRate();
    GUARD_LOG(frameRate > 0, "ERROR. Frame rate not defined")

    auto textures = FindTextures(*owner->World(), models);
    VectorList<SP<SomeTexture>> textureList(textures.begin(), textures.end());
    SetTextures(textureList);

    TimeTrack<int>::Config config{ .id = "frame.playable",
                                   .duration = 0,
                                   .cycleType = CycleType(),
                                   .keyedTimeType = KeyedTimeType::Discrete };
    auto track = NEW<TimeTrack<int>>(config);

    float frameTime = 1.0f / frameRate;

    float time = 0;
    bool isFirst = true;
    for (auto& model : models) {
        if (model.deltaTime) {
            time += *model.deltaTime;
        } else if (model.deltaFrame) {
            time += *model.deltaFrame * frameTime;
        } else if (!isFirst) {
            time += frameTime;
        }

        isFirst = false;

        String textureId = model.textureId;
        auto frame = FrameForTextureId(textureId);
        GUARD_CONTINUE(frame)

        track->AddAt(time)->SetValue(*frame);
    }

    // Hold final frame based on frame rate
    track->SetDuration(time + frameTime);

    framePlayable = NEW<FramePlayable>(track);

    // Synchronize states
    OnFrameChange();
}

std::optional<int> AnimatedSpriteRenderer::FrameForTextureId(String textureId) const {
    for (size_t i = 0; i < frames.size(); i++) {
        auto& frame = frames[i];
        GUARD_CONTINUE(frame.texture->id == textureId)

        return (int)i;
    }

    return {};
}
