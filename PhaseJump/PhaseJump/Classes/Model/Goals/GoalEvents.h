#ifndef PJGOALEVENT_H
#define PJGOALEVENT_H

#include "Event.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 5/11/24
 */
namespace PJ
{
    class SomeGoal;

    /// <summary>
    /// Event: goal has been completed
    /// </summary>
    class GoalCompleteEvent : public Event
    {
    public:
        SP<SomeGoal> goal;

        GoalCompleteEvent(SP<SomeGoal> goal) : goal(goal)
        {
        }
    };
}

#endif
