#ifndef PJTIMEDSEQUENCE_H
#define PJTIMEDSEQUENCE_H

#include "TimedPlayable.h"
#include "Updatable.h"
#include "Utils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 1/12/23
 */
namespace PJ {
    /// Run a series of Timed objects in sequence
    class TimedSequence : public Updatable {
    protected:
        // TODO: SP audit
        VectorList<SP<TimedPlayable>> sequence;
        int index = 0;

    public:
        void Add(SP<TimedPlayable> timed) {
            PJ::Add(sequence, timed);
        }

        void OnUpdate(TimeSlice time) override {
            if (isFinished) {
                return;
            }

            auto timeDelta = time.delta;

            while (timeDelta > 0) {
                bool isValidIndex = (index >= 0 && index < sequence.size());
                if (!isValidIndex) {
                    break;
                }

                auto timed = sequence[index];
                float timeSpent = timed->Progress() * timed->duration;
                float timeLeft = timed->duration - timeSpent;
                float spendTime = std::min(timeLeft, timeDelta);
                timed->OnUpdate(TimeSlice(spendTime));
                timeDelta -= spendTime;

                if (!timed->IsFinished()) {
                    break;
                }
                index++;
            }

            isFinished = index >= sequence.size();
        }
    };
} // namespace PJ

#endif
