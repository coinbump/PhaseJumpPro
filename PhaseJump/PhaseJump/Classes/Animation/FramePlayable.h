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
    class FramePlayable final : public SomeFramePlayable {
    public:
        using Base = SomeFramePlayable;
        using This = FramePlayable;

    protected:
        float frameRate{};

        /// Stores keyframes for the frame positions
        UP<TimeTrack<int>> track;

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

        /// @return Best-guess frame rate computed from the average interval between consecutive
        /// keyframes on the underlying track. Falls back to frameRate property
        float FrameRate() const override;

        /// Sets the frame rate for fixed-rate animations by redistributing the track's keyframes
        /// at uniform intervals and updating the duration
        void SetFrameRate(float value) override;

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
    };
} // namespace PJ
