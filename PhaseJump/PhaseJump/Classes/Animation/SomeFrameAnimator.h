#pragma once

#include "TimeSlice.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/12/24
 */
namespace PJ {
    // FUTURE: support CycleFrameAnimator if needed (for loop, ping-pong, etc.)
    // FUTURE: support PositionFrameAnimator if needed (for complex frame timelines)
    /// Animates a frame value (for frame animations)
    class SomeFrameAnimator {
    public:
        virtual ~SomeFrameAnimator() {}

        virtual int FrameForUpdate(TimeSlice time, int frameCount) = 0;
    };

    /// Animates by a fixed frame rate
    class RateFrameAnimator : public SomeFrameAnimator {
    protected:
        float position = 0;
        int frame = 0;

    public:
        /// Frames per second
        float rate = 5.0f;

        RateFrameAnimator(float rate) : rate(rate) {
        }

        int FrameForUpdate(TimeSlice time, int frameCount) override;
    };
}
