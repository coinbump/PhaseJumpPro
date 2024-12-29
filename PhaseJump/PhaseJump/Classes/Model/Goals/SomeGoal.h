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
 CODE REVIEW: 12/29/24
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

        virtual void OnComplete() {}
    };

    /// Goal with a core and funcs
    template <class Core = Void>
    class Goal : public SomeGoal {
    public:
        using Base = SomeGoal;
        using This = Goal<Core>;

        using GoalFunc = std::function<void(This&)>;

        GoalFunc onCompleteFunc;

        Core core{};

        Goal(Core core = {}) :
            core(core) {}

        void OnComplete() override {
            GUARD(onCompleteFunc);
            onCompleteFunc(*this);
        }
    };
} // namespace PJ
