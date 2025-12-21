#pragma once

#include "Goal.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/29/24
 */
namespace PJ {
    /// A goal that must reach a value target to be complete
    class ValueTargetGoal : public Goal {
    protected:
        int valueTarget{};
        int value{};

    public:
        int ValueTarget() const {
            return valueTarget;
        }

        void SetValueTarget(int value) {
            valueTarget = value;
        }

        int Value() const {
            return value;
        }

        void SetValue(int value) {
            this->value = value;
            SetIsComplete(value >= valueTarget);
        }

        ValueTargetGoal(String id, int valueTarget) :
            valueTarget(valueTarget) {
            this->id = id;
        }

        void Increment() {
            GUARD(value < valueTarget)
            SetValue(Value() + 1);
        }

        // MARK: Goal

        float Progress() const override {
            return std::clamp((float)value / (float)valueTarget, 0.0f, 1.0f);
        }
    };
} // namespace PJ
