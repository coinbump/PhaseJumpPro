#pragma once

#include "DiagnoseModel.h"
#include "SomeFramePlayable.h"
#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "TimeTrack.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    class RenderMaterial;
    struct Mesh;
    class TextureAtlas;

    /// Renders multiple textures as a sprite
    class AnimatedSpriteRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;
        using This = AnimatedSpriteRenderer;
        using TextureList = VectorList<SP<SomeTexture>>;

        /// Model to build animation keyframe
        struct KeyframeModel {
            String textureId;

            /// If specified, the relative postion of the next keyframe
            std::optional<float> deltaTime;

            /// If specified, the relative position of the next keyframe
            std::optional<int> deltaFrame;

            // If nothing is specified, 1 frame is advanced for new keyframes, based on the
            // playable's frame rate
        };

        /// Used to enable extra logging for this object
        DiagnoseModel _diagnose;

    protected:
        /// Model for animation frames
        struct Frame {
            SP<SomeTexture> texture;
            Vector2 offset;

            Frame(SP<SomeTexture> texture, Vector2 offset) :
                texture(texture),
                offset(offset) {}
        };

        int frame = 0;
        bool flipX = false;
        bool flipY = false;

        VectorList<Frame> frames;

        /// Keyframe models for startup
        VectorList<KeyframeModel> keyframeModels;

        AnimatedSpriteRenderer(Vector3 worldSize, float frameRate, AnimationCycleType cycleType);

        void Configure();

    public:
        UP<SomeFramePlayable> framePlayable;

        static constexpr float DefaultFrameRate = 24.0f;

        AnimatedSpriteRenderer(
            TextureList const& textures, float frameRate = DefaultFrameRate,
            AnimationCycleType cycleType = AnimationCycleType::Loop
        );
        AnimatedSpriteRenderer(
            TextureAtlas const& atlas, float frameRate = DefaultFrameRate,
            AnimationCycleType cycleType = AnimationCycleType::Loop
        );
        AnimatedSpriteRenderer(
            VectorList<KeyframeModel> const& models, float frameRate = DefaultFrameRate,
            AnimationCycleType cycleType = AnimationCycleType::Loop
        );

        Vector2 Size() const;

        void SetCycleType(AnimationCycleType cycleType) {
            GUARD(framePlayable)
            framePlayable->SetCycleType(cycleType);
        }

        void SetFlipX(bool value) {
            GUARD(flipX != value)
            flipX = value;
            model.SetRenderModelsNeedBuild();
        }

        void SetFlipY(bool value) {
            GUARD(flipY != value)
            flipY = value;
            model.SetRenderModelsNeedBuild();
        }

        void SetFrame(int value);

        float FrameRate() const {
            GUARDR(framePlayable, 0)
            return framePlayable->FrameRate();
        }

        void SetFrameRate(float value) {
            GUARD(framePlayable)
            framePlayable->SetFrameRate(value);
        }

        void SetTextures(VectorList<SP<SomeTexture>> const& textures);

        std::optional<int> FrameForTextureId(String textureId) const;

        /// Set the frame playable to use these frames and frame rates
        void SetFrames(VectorList<KeyframeModel> models);

        AnimationCycleType CycleType() const {
            GUARDR(framePlayable, {})
            return framePlayable->CycleType();
        }

        // MARK: SomeWorldComponent

        void Awake() override;

        String TypeName() const override {
            return "AnimatedSpriteRenderer";
        }

        // MARK: SomeRenderer

        void Reset() override;

        bool IsRenderFinished() const override {
            return framePlayable && framePlayable->IsFinished();
        }

        // MARK: WorldComponent

        void OnUpdate(TimeSlice time) override;

    protected:
        virtual void OnFrameChange();
    };

    using AnimatedTextureRenderer = AnimatedSpriteRenderer;
} // namespace PJ
