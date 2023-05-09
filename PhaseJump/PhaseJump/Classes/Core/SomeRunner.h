#ifndef PJSOMERUNNER_H
#define PJSOMERUNNER_H

#include "Updatable.h"

/*
 RATING: 5 stars
 Has unit tests (via Timer)
 CODE REVIEW: 12/19/22
 */
namespace PJ {
    /// <summary>
    /// An object that runs and then may finish or start over
    /// </summary>
    class SomeRunner : public Updatable {
    protected:
        /// <summary>
        /// If true, we are finished running
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
