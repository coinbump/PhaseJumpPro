#pragma once

#include "AnimationCycleTimer.h"
#include "SomeFramePlayable.h"
#include "TimeTrack.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/30/24
 */
namespace PJ {
    /// Animates frames using a time track with frame values
    class FramePlayable : public SomeFramePlayable {
    public:
        using Base = SomeFramePlayable;
        using This = FramePlayable;

    protected:
        float frameRate{};

        /// Stores keyframes for the frame positions
        UP<TimeTrack<int>> track;
        Playable* controller{};

    public:
        /// Constructs with a fixed frame rate
        FramePlayable(int frameCount, float frameRate, AnimationCycleType cycleType);
        FramePlayable(UP<TimeTrack<int>>& track);

        /// @return Returns the number of frames
        size_t FrameCount() const {
            GUARDR(track, 0)
            return track->Keyframes().size();
        }

        /// @return Returns the duration of a single frame for fixed rate animations
        float FrameTime() const {
            auto frameCount = FrameCount();
            GUARDR(frameCount > 0, 0)
            return Duration() / frameCount;
        }

        // MARK: SomeFramePlayable

        /// @return Returns the frame rate for fixed rate animations
        float FrameRate() const override {
            return frameRate;
        }

        /// Set the frame rate for fixed rate animations
        void SetFrameRate(float value) override {
            frameRate = value;

            auto frameCount = FrameCount();
            GUARD(value >= 0)
            SetDuration((1.0f / value) * frameCount);
        }

        AnimationCycleType CycleType() const override {
            GUARDR(track, {})
            return track->CycleType();
        }

        int Frame() const override;

        void SetCycleType(AnimationCycleType cycleType) override {
            GUARD(track)

            track->SetCycleType(cycleType);
            track->Reset();
        }

        // MARK: Playable

        Playable* Controller() const override {
            return controller;
        }
    };
} // namespace PJ
