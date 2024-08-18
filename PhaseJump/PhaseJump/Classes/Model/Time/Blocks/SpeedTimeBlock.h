#pragma once

#include "TimeBlock.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: reconsider with composition
    // TODO: needs unit tests
    /// Multiplies time delta by speed
    class SpeedTimeBlock : public TimeBlock {
    public:
        using Base = TimeBlock;

        float speed = 1.0f;

        SpeedTimeBlock(float speed, UP<TimeBlock>& child) :
            Base(child),
            speed(speed) {}

        void OnUpdateChild(TimeSlice time) override {
            Base::OnUpdateChild(time * std::max(speed, 0));
        }

        void OnUpdateBlock(TimeSlice time) override {}
    };
} // namespace PJ
