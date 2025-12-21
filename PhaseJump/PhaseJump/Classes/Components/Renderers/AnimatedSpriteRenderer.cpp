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
#include "ResourceCatalog.h"
#include "ShaderProgram.h"
#include "SomeRenderEngine.h"
#include "TextureAtlas.h"
#include "UIPlanner.h"
#include "Utils.h"
#include "World.h"

using namespace std;
using namespace PJ;

UnorderedSet<SP<Texture>>
_FindTextures(World& world, VectorList<AnimatedSpriteRenderer::KeyframeModel> const& models) {
    VectorList<String> textureIds =
        Map<String>(models, [](auto& model) { return model.textureId; });
    textureIds = Filter(textureIds, [](auto& textureId) { return textureId.length() > 0; });

    UnorderedSet<String> textureIdSet(textureIds.begin(), textureIds.end());
    return world.resources.TypeSet<Texture>(ResourceType::Texture, textureIdSet);
}

void AnimatedSpriteRenderer::SetTextures(VectorList<SP<Texture>> const& textures) {
    frames.clear();

    std::transform(
        textures.cbegin(), textures.cend(), std::back_inserter(frames),
        [](auto& texture) {
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

AnimatedSpriteRenderer::AnimatedSpriteRenderer(Config const& config) :
    Base({}) {

    model.material =
        MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::TextureVary });

    model.SetBuildMeshFunc([this](auto& model) {
        QuadMeshBuilder builder(model.WorldSize());
        auto result = builder.BuildMesh();

        GUARDR(frame >= 0 && frame < frames.size(), result)
        auto frameModel = frames[frame];

        auto offset = frameModel.offset;
        if (flipX) {
            offset.x = -offset.x;
        }
        if (flipY) {
            offset.y = -offset.y;
        }

        result.Offset(offset);

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

        GUARDR(model.Material(), result)
        auto& textures = model.Material()->Textures();
        if (!IsEmpty(textures)) {
            UVTransformFuncs::textureCoordinates(*textures[0], result.UVs());
        }

        return result;
    });

    model.material->EnableFeature(RenderFeature::Blend, true);

    auto textures = config.textures;

    if (config.atlas) {
        auto& atlas = *config.atlas;
        VectorList<SP<Texture>> someTextures(atlas.Textures().begin(), atlas.Textures().end());
        textures = someTextures;
    }

    SetTextures(textures);
    framePlayable = NEW<FramePlayable>(textures.size(), config.frameRate, config.cycleType);

    // Keyframes require textures, so wait until Awake when we have a world
    keyframeModels = config.keyframeModels;

    SetCycleType(config.cycleType);

    // Synchronize states
    OnFrameChange();

    PlanUIFunc planUIFunc = [this](auto args) {
        auto& planner = args.planner;

        planner
            .InputBool({ .label = "Flip X",
                         .binding = { [this]() { return flipX; },
                                      [this](auto& value) { SetFlipX(value); } } })
            .InputBool({ .label = "Flip Y",
                         .binding = { [this]() { return flipY; },
                                      [this](auto& value) { SetFlipY(value); } } });

        FramePlayable* rateFramePlayable = dynamic_cast<FramePlayable*>(framePlayable.get());
        if (rateFramePlayable) {
            VectorList<String> cycleOptions{ "Once", "PingPong", "Loop" };
            planner
                .InputFloat({ .label = "Frame rate",
                              .binding = {
                                  [=]() { return rateFramePlayable->FrameRate(); },
                                  [=](auto& value) { rateFramePlayable->SetFrameRate(value); } } })
                .PickerList({ .label = "Animation cycle",
                              .options = cycleOptions,
                              .binding = { [=]() { return (int)rateFramePlayable->CycleType(); },
                                           [=](auto& value) {
                                               rateFramePlayable->SetCycleType((AnimationCycleType
                                               )value);
                                           } } });
        }
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);

    Configure();
}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(
    TextureList const& textures, float frameRate, AnimationCycleType cycleType
) :
    AnimatedSpriteRenderer(Config{
        .textures = textures, .frameRate = frameRate, .cycleType = cycleType }) {}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(
    TextureAtlas const& atlas, float frameRate, AnimationCycleType cycleType
) :
    AnimatedSpriteRenderer(Config{ .atlas = &atlas, .frameRate = frameRate, .cycleType = cycleType }
    ) {}

AnimatedSpriteRenderer::AnimatedSpriteRenderer(
    VectorList<KeyframeModel> const& models, float frameRate, AnimationCycleType cycleType
) :
    AnimatedSpriteRenderer(Config{
        .keyframeModels = models, .frameRate = frameRate, .cycleType = cycleType }) {}

void AnimatedSpriteRenderer::Awake() {
    if (!IsEmpty(keyframeModels)) {
        SetFrames(keyframeModels);
        keyframeModels.clear();
    }
}

void AnimatedSpriteRenderer::Configure() {
    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<RenderModel> result;

        auto material = model.material.get();
        GUARDR_LOG(material, result, "ERROR. Missing material")

        GUARDR(owner, result)
        GUARDR(frame >= 0 && frame < frames.size(), result)
        auto frameModel = frames[frame];

        auto renderModel = RenderModelBuilder().Build(*this, model);
        GUARDR(renderModel, result)

        renderModel->SetVertexColors(std::span<RenderColor const>(model.VertexColors()));

        result.push_back(*renderModel);
        return result;
    });
}

void AnimatedSpriteRenderer::Reset() {
    Base::Reset();

    SetFrame(0);
}

Vector2 AnimatedSpriteRenderer::Size() const {
    GUARDR(frame >= 0 && frame < frames.size(), {})
    auto const& texture = frames[frame].texture;
    auto textureSize = texture->Size();

    return Vector2(textureSize.x, textureSize.y);
}

FinishType AnimatedSpriteRenderer::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARDR(!IsEmpty(frames), FinishType::Continue)
    GUARDR(framePlayable, FinishType::Continue)

    framePlayable->OnUpdate(time);
    auto frame = framePlayable->Frame();

    SetFrame(frame);

    return FinishType::Continue;
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
    model.SetMeshNeedsBuild();

    PJ::LogIf(_diagnose, "Frame Change: ", MakeString(frame), " texture: ", frameTexture->id);
}

void AnimatedSpriteRenderer::SetFrames(VectorList<KeyframeModel> models) {
    GUARD_LOG(owner && owner->World(), "ERROR. Need world for SetFrames to find textures")

    auto frameRate = framePlayable->FrameRate();
    GUARD_LOG(frameRate > 0, "ERROR. Frame rate not defined")

    auto textures = _FindTextures(*owner->World(), models);
    VectorList<SP<Texture>> textureList(textures.begin(), textures.end());
    SetTextures(textureList);

    TimeTrack<int>::Config const& config{ .id = "frame.playable",
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

void AnimatedSpriteRenderer::SetFlipX(bool value) {
    GUARD(flipX != value)
    flipX = value;
    model.SetMeshNeedsBuild();
}

void AnimatedSpriteRenderer::SetFlipY(bool value) {
    GUARD(flipY != value)
    flipY = value;
    model.SetMeshNeedsBuild();
}

float AnimatedSpriteRenderer::FrameRate() const {
    GUARDR(framePlayable, 0)
    return framePlayable->FrameRate();
}

void AnimatedSpriteRenderer::SetFrameRate(float value) {
    GUARD(framePlayable)
    framePlayable->SetFrameRate(value);
}
