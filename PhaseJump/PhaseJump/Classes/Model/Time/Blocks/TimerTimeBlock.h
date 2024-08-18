#pragma once

#include "TimeBlock.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: reconsider with composition
    // TODO: needs unit tests
    class TimerTimeBlock : public TimeBlock {
    protected:
        float duration = 0;

        /// Time value
        float state = 0;

        /// Time left over from last update
        float timeOverflow = 0;

    public:
        using Base = TimeBlock;

        TimerTimeBlock(float duration) :
            duration(duration) {}

        void Reset() override {
            state = 0;
            timeOverflow = 0;
            isFinished = false;
        }

        void OnUpdateBlock(TimeSlice time) override {
            GUARD(!isFinished)

            Base::OnUpdateBlock(time);

            state += (time.delta + timeOverflow);
            timeOverflow = 0;

            // Keep track of overflows so our timers are precise
            if (state >= duration) {
                timeOverflow = state % duration;
                Finish();
            }
        }
    };
} // namespace PJ
