#pragma once

#include "Broadcaster.h"
#include "GoalEvents.h"
#include "StandardCore.h"
#include "StringUtils.h"
#include "Void.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/19/25
 */
namespace PJ {
    /// Goals are common in games to achieve progress.
    /// EXAMPLE: complete a task N times, achieve N experience, get to the
    /// destination, etc.
    class Goal : public Base {
    public:
        using This = Goal;
        using GoalFunc = std::function<void(This&)>;

        GoalFunc onCompleteFunc;

        String id;

    private:
        bool isComplete = false;

    public:
        bool IsComplete() const {
            return isComplete;
        }

        void SetIsComplete(bool value) {
            GUARD(isComplete != value)

            isComplete = value;
            GUARD(isComplete)

            OnComplete();
        }

        void Complete() {
            SetIsComplete(true);
        }

        virtual float Progress() const {
            return isComplete ? 1 : 0;
        }

        virtual void OnComplete() {
            GUARD(onCompleteFunc);
            onCompleteFunc(*this);
        }
    };

    /// Goal with a core and funcs
    template <class Core = Void>
    class CoreGoal : public Goal {
    public:
        using Base = Goal;
        using This = CoreGoal<Core>;

        Core core{};

        CoreGoal(Core const& core = {}) :
            core(core) {}
    };
} // namespace PJ
