#pragma once

#include "SomeSignal.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    class SomeGoal;

    /// Event: goal has been completed
    class GoalCompleteEvent : public SomeSignal {
    public:
        SP<SomeGoal> goal;

        GoalCompleteEvent(SP<SomeGoal> goal) :
            goal(goal) {}
    };
} // namespace PJ
