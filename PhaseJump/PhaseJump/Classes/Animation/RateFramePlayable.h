#pragma once

#include "AnimationCycleTimer.h"
#include "SomeFramePlayable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Animates by a fixed frame rate
    class RateFramePlayable : public SomeFramePlayable {
    public:
        using Base = SomeFramePlayable;
        using This = RateFramePlayable;

    protected:
        int frameCount = 0;

        AnimationCycleTimer timer;
        Playable* playDriver = &timer;

    public:
        RateFramePlayable(int frameCount, float rate) :
            timer(rate > 0 ? frameCount * (1.0f / rate) : 0, AnimationCycleType::Loop),
            frameCount(frameCount) {
            timer.SetOnPlayTimeChangeFunc([this](auto& playable) { OnPlayTimeChange(); });
        }

        float FrameTime() const {
            GUARDR(frameCount > 0, 0)
            return Duration() / frameCount;
        }

        float FrameRate() const {
            auto frameTime = FrameTime();
            GUARDR(frameTime > 0, 0)
            return 1.0f / FrameTime();
        }

        void SetFrameRate(float value) {
            GUARD(value >= 0)
            SetDuration((1.0f / value) * frameCount);
        }

        AnimationCycleType CycleType() const {
            return timer.CycleType();
        }

        void SetCycleType(AnimationCycleType value) {
            GUARD(timer.CycleType() != value)
            timer.SetCycleType(value);
            timer.Reset();
        }

        // MARK: SomeFramePlayable

        int Frame() const override;

        // MARK: Playable

        Playable* PlayDriver() const override {
            return playDriver;
        }
    };
} // namespace PJ
