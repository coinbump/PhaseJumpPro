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
    class AnimatedSpriteRenderer : public SomeMaterialRenderer {
    public:
        using Base = SomeMaterialRenderer;
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

        int frame{};
        bool flipX{};
        bool flipY{};

        VectorList<Frame> frames;

        /// Keyframe models for startup
        VectorList<KeyframeModel> keyframeModels;

        void Configure();

    public:
        UP<SomeFramePlayable> framePlayable;

        static constexpr float DefaultFrameRate = 24.0f;

        struct Config {
            /// Configuration: textures
            TextureList textures;

            /// Alt Configuration: texture atlas
            TextureAtlas const* atlas{};

            /// Alt Configuration: keyframe models
            VectorList<KeyframeModel> keyframeModels;

            float frameRate = DefaultFrameRate;
            AnimationCycleType cycleType = AnimationCycleType::Loop;
        };

        AnimatedSpriteRenderer(Config config);
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

        /// Sets the x flip value. If true, the sprite will be flipped horizontally
        void SetFlipX(bool value);

        /// Sets the y flip value. If true, the sprite will be flipped vertically
        void SetFlipY(bool value);

        /// Sets the current frame, by index
        void SetFrame(int value);

        /// @return Returns the current frame rate (might not be accurate because of timeline
        /// keyframe changes)
        float FrameRate() const;

        /// Sets the frame rate
        void SetFrameRate(float value);

        /// Set the animation frame textures
        void SetTextures(VectorList<SP<SomeTexture>> const& textures);

        /// @return Returns the frame for a specific texture id, if any
        std::optional<int> FrameForTextureId(String textureId) const;

        /// Sets the frame playable to use these frames and frame rates
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
