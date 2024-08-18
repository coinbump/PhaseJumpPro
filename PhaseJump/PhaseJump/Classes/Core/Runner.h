#pragma once

#include "Updatable.h"

/*
 Has unit tests (via Timer)
 CODE REVIEW: TODO:
 */
namespace PJ {
    /// An object that runs and then may finish or start over
    /// Allows for repeating timers
    class Runner {
    public:
        // TODO: use composition for behavior, not enums
        enum class RunType {
            // Destroy when finished
            RunOnce,

            // Repeats (example: timer that does an update every N seconds)
            Repeat
        };

        RunType runType;

        std::function<void(Runner&)> onResetFunc;
        std::function<void(Runner&)> onFinishFunc;

    protected:
        /// If true, we are finished running
        bool isFinished = false;

        virtual void OnFinish() {
            GUARD(onFinishFunc)
            onFinishFunc(*this);
        }

    public:
        Runner(RunType runType) :
            runType(runType) {}

        virtual ~Runner() {}

        bool IsFinished() const {
            return isFinished;
        }

        void SetIsFinished(bool value) {
            if (value == isFinished) {
                return;
            }
            isFinished = value;

            if (!isFinished) {
                return;
            }
            OnFinish();

            switch (runType) {
            case RunType::Repeat:
                Reset();
                break;
            default:
                break;
            }
        }

        virtual void Reset() {
            isFinished = false;
            OnReset();
        }

        virtual void OnReset() {
            GUARD(onResetFunc)
            onResetFunc(*this);
        }
    };
} // namespace PJ
