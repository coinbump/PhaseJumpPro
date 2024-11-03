#pragma once

#include "AnimationCycleTypes.h"
#include "Playable.h"

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 9/25/24
 */
namespace PJ {
    /// Animates a frame value (for frame animations)
    class SomeFramePlayable : public Playable {
    public:
        SomeFramePlayable() {}

        virtual ~SomeFramePlayable() {}

        int FrameForUpdate(TimeSlice time) {
            OnUpdate(time);
            return Frame();
        }

        virtual int Frame() const = 0;
        virtual float FrameRate() const = 0;
        virtual void SetFrameRate(float value) = 0;
        virtual AnimationCycleType CycleType() const = 0;
        virtual void SetCycleType(AnimationCycleType cycleType) = 0;
    };
} // namespace PJ
