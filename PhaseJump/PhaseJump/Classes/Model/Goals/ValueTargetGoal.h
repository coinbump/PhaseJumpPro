#pragma once

#include "SomeGoal.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// A goal that must reach a value target to be complete
    class ValueTargetGoal : public SomeGoal {
    protected:
        int valueTarget = 0;
        int value = 0;

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

        float Progress() const override {
            return std::clamp((float)value / (float)valueTarget, 0.0f, 1.0f);
        }

        void Increment() {
            if (value >= valueTarget) {
                return;
            }

            SetValue(Value() + 1);
        }
    };
} // namespace PJ
