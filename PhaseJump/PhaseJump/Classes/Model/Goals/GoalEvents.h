#ifndef PJGOALEVENT_H
#define PJGOALEVENT_H

#include "Event.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 12/29/22
 */
namespace PJ
{
    class SomeGoal;

    /// <summary>
    /// Event: goal has been completed
    /// </summary>
    class EventGoalComplete : public Event
    {
    public:
        std::shared_ptr<SomeGoal> goal;

        EventGoalComplete(std::shared_ptr<SomeGoal> goal) : goal(goal)
        {
        }
    };
}

#endif
