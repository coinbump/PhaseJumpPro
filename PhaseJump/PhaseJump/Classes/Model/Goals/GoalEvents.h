#ifndef PJGOALEVENT_H
#define PJGOALEVENT_H

#include "SomeEvent.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 5/11/24
 */
namespace PJ {
    class SomeGoal;

    /// Event: goal has been completed
    class GoalCompleteEvent : public SomeEvent {
    public:
        // TODO: SP-audit
        SP<SomeGoal> goal;

        GoalCompleteEvent(SP<SomeGoal> goal) :
            goal(goal) {}
    };
} // namespace PJ

#endif
