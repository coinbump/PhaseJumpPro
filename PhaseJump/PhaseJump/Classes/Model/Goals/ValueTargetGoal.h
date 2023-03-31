#ifndef PJVALUETARGETGOAL_H
#define PJVALUETARGETGOAL_H

#include "SomeGoal.h"
#include <algorithm>

/*
 RATING: 5 stars
 Hase unit tests
 CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// A goal that must reach a value target to be complete
    /// </summary>
    class ValueTargetGoal : public SomeGoal
    {
    protected:
        int valueTarget = 0;
        int value = 0;

    public:
        int ValueTarget() const { return valueTarget; }
        void SetValueTarget(int value) { valueTarget = value; }
        int Value() const { return value; }
        void SetValue(int value) {
            this->value = value;
            SetIsComplete(value >= valueTarget);
        }

        ValueTargetGoal(String id, int valueTarget, int value = 0) : valueTarget(valueTarget), value(value)
        {
            this->id = id;
            SetIsComplete(value >= valueTarget);
        }

        float Progress() const override {
            return std::max(0.0f, std::min(1.0f, (float)value / (float)valueTarget));
        }

        void Increment()
        {
            if (value >= valueTarget) { return; }

            SetValue(Value() + 1);
        }
    };
}

#endif
