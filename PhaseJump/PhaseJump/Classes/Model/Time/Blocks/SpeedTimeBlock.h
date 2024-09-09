#pragma once

#include "TimeBlock.h"
#include "Updatable.h"

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

    /// Multiplies time delta by speed
    class SpeedUpdatable : public {
    public:
        using Base = Updatable;

        Updatable funcUpdatable;

        float speed = 1.0f;

        SpeedUpdatable(float speed, Updatable::OnUpdateFunc _onUpdateFunc) :
            funcUpdatable([this](TimeSlice time) {
                float delta = std::max(speed, 0);
                return _onUpdateFunc(time * std::max(speed, 0));
            }),
            speed(speed) {}

        void OnUpdate(TimeSlice time) override {
            funcUpdatable.OnUpdate(time);
        }
    };

} // namespace PJ
