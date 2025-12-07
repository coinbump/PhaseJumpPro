#pragma once

#include "Updatable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    enum class RunType {
        /// Run one time
        Once,

        /// Repeats (restarts after finish)
        Repeat
    };

    /// Manages logic for running an updatable that when it finishes, either repeats or finishes
    class Runner {
    public:
        using OnResetFunc = std::function<void()>;
        using OnFinishFunc = std::function<void()>;

        RunType runType;

        OnResetFunc onResetFunc;
        OnFinishFunc onFinishFunc;

    protected:
        /// If true, we are finished running
        bool isFinished = false;

    public:
        Runner(RunType runType) :
            runType(runType) {}

        void Reset() {
            GUARD(isFinished)
            isFinished = false;

            GUARD(onResetFunc)
            onResetFunc();
        }

        void SetIsFinished(bool value) {
            GUARD(value != isFinished)
            isFinished = value;

            GUARD(isFinished)
            if (onFinishFunc) {
                onFinishFunc();
            }

            switch (runType) {
            case RunType::Repeat:
                Reset();
                break;
            default:
                break;
            }
        }

        bool IsFinished() const {
            return isFinished;
        }
    };
} // namespace PJ
