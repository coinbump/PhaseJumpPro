#pragma once

#include "SomeFramePlayable.h"
#include "TimeTrack.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/28/24
 */
namespace PJ {
    /// Animates frames by keyframes along a time track
    class TrackFramePlayable : public SomeFramePlayable {
    public:
        using Base = SomeFramePlayable;
        using This = TrackFramePlayable;

    protected:
        TimeTrack<int, Keyframe<int>> timeTrack;
        Playable* playDriver = &timeTrack;

        int frame = 0;

    public:
        TrackFramePlayable(
            float duration, AnimationCycleType cycleType = AnimationCycleType::Loop
        ) :
            timeTrack(duration, cycleType, TimeTrackValueType::Discrete) {
            timeTrack.setBindingFunc = [this](auto& value) { frame = value; };
        }

        void AddFrameAt(int frame, float time) {
            timeTrack.AddAt(time)->value = frame;
        }

        // MARK: SomeFramePlayable

        int Frame() const override {
            return frame;
        }

        // MARK: Playable

        Playable* PlayDriver() const override {
            return playDriver;
        }
    };
} // namespace PJ
