#ifndef PJSOMEGOAL_H
#define PJSOMEGOAL_H

#include "_String.h"
#include "Core.h"
#include "GoalEvents.h"
#include "Broadcaster.h"
#include <memory>

/*
 RATING: 5 stars
 Hase unit tests
 CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Goals are common in games to achieve progress.
    /// EXAMPLE: complete a task N times, achieve N experience, get to the destination, etc.
    /// </summary>
    class SomeGoal : public Base
    {
    public:
        String id;

    private:
        bool isComplete = false;

    public:
        bool IsComplete() const { return isComplete; }
        void SetIsComplete(bool value)
        {
            if (isComplete == value) { return; }
            isComplete = value;
            if (isComplete)
            {
                OnComplete();
            }
        }

        Broadcaster broadcaster;

        virtual float Progress() const = 0;

        virtual void OnComplete()
        {
            broadcaster.Broadcast(MAKE<GoalCompleteEvent>(SCAST<SomeGoal>(shared_from_this())));
        }
    };
}

#endif
