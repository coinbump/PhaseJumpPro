#ifndef PJTIMEDSEQUENCE_H
#define PJTIMEDSEQUENCE_H

#include "Updatable.h"
#include "VectorList.h"
#include "SomeTimed.h"
#include "Macros.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 1/12/24
 */
namespace PJ
{
    /// <summary>
    /// Run a series of Timed objects in sequence
    /// </summary>
    class TimedSequence : public Updatable
    {
    protected:
        VectorList<SP<SomeTimed>> sequence;
        int index = 0;
        bool isFinished = false;

    public:
        bool IsFinished() const override { return isFinished; };

        void Add(SP<SomeTimed> timed)
        {
            sequence.Add(timed);
        }

        void OnUpdate(TimeSlice time) override
        {
            if (isFinished) { return; }

            auto timeDelta = time.delta;

            while (timeDelta > 0)
            {
                bool isValidIndex = (index >= 0 && index < sequence.size());
                if (!isValidIndex) { break; }

                auto timed = sequence[index];
                float timeSpent = timed->Progress() * timed->duration;
                float timeLeft = timed->duration - timeSpent;
                float spendTime = std::min(timeLeft, timeDelta);
                timed->OnUpdate(TimeSlice(spendTime));
                timeDelta -= spendTime;

                if (!timed->IsFinished()) { break; }
                index++;
            }

            isFinished = index >= sequence.size();
        }
    };
}

#endif
