#ifndef PJSOMEGOAL_H
#define PJSOMEGOAL_H

#include "_String.h"
#include "Broadcaster.h"
#include "GoalEvents.h"
#include "StandardCore.h"
#include <memory>

/*
 RATING: 5 stars
 Hase unit tests
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    /// Goals are common in games to achieve progress.
    /// EXAMPLE: complete a task N times, achieve N experience, get to the
    /// destination, etc.
    class SomeGoal : public Base {
    public:
        String id;

    private:
        bool isComplete = false;

    public:
        bool IsComplete() const {
            return isComplete;
        }

        void SetIsComplete(bool value) {
            if (isComplete == value) {
                return;
            }
            isComplete = value;
            if (isComplete) {
                OnComplete();
            }
        }

        void Complete() {
            SetIsComplete(true);
        }

        virtual float Progress() const = 0;

        virtual void OnComplete() {}
    };
} // namespace PJ

#endif
