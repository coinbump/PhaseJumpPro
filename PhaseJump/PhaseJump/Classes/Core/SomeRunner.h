#ifndef PJSOMERUNNER_H
#define PJSOMERUNNER_H

#include "Core/Updatable.h"

/*
 RATING: 5 stars
 Has unit tests (via Timer)
 CODE REVIEW: 12/19/22
 */
namespace PJ {
    /// <summary>
    /// Any object that relies on timers to perform tasks
    ///
    /// Supports an internal time scale for animating an object's time
    /// </summary>
    class SomeRunner : public Updatable {
    protected:
        /// <summary>
        /// If true, timer has reached the end of its duration
        /// </summary>
        bool isFinished = false;

        virtual void OnFinish() {}

    public:
        enum class RunType {
            // Destroy when finished
            RunOnce,

            // Repeats (example: timer that does an update every N seconds)
            KeepRunning
        };

        RunType runType;

        SomeRunner(RunType runType) : runType(runType) {
        }
        virtual ~SomeRunner() {}

        bool IsFinished() const override
        {
            return isFinished;
        }

        void SetIsFinished(bool value) {
            if (value != isFinished) {
                isFinished = value;

                if (isFinished) {
                    OnFinish();

                    switch (runType)
                    {
                        case RunType::KeepRunning:
                            Reset();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        virtual void Reset()
        {
            isFinished = false;
        }
    };
}

#endif
