#pragma once

#include "SomeFramePlayable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 6/14/24
 */
namespace PJ {
    /// Animates by a fixed frame rate
    class RateFramePlayable : public SomeFramePlayable {
    protected:
        int frameCount = 0;

        /// Frames per second
        float rate = 5.0f;

    public:
        RateFramePlayable(int frameCount, float rate) :
            frameCount(frameCount),
            rate(rate) {
            UP<SomePlayTimeController> playTimeController(
                new LoopPlayTimeController(*this, Duration())
            );
            this->playTimeController = std::move(playTimeController);
        }

        float FrameTime() const {
            return 1.0f / rate;
        }

        // MARK: SomeFrameAnimator

        int Frame() const override;

        // MARK: Playable

        float Duration() const override {
            return FrameTime() * frameCount;
        }

        float Progress() const override {
            return PlayTime() / Duration();
        }

        float PlayTime() const override {
            return playTimeController ? playTimeController->Position() : 0;
        }

        void SetPlayTime(float time) override {
            GUARD(playTimeController);
            playTimeController->SetPosition(time);
        }
    };
} // namespace PJ
