#ifndef PJTIMER_H_
#define PJTIMER_H_

#include "SomeTimed.h"
#include <algorithm>
#include <string>

/*
 * RATING: 5 stars
 * Simple timer with unit tests
 * CODE REVIEW: 11/5/22
 */
namespace PJ {
	/// <summary>
    /// Keeps track of time for N seconds duration, and then triggers OnFinish
    /// </summary>
	class Timer : public SomeTimed
	{
    protected:
        float timerState = 0;  // Time state of the timer

    public:
        /// <summary>
        /// Optional id for hashing, debug logs
        /// </summary>
        std::string id;

        Timer(float duration, RunType runType)
        : SomeTimed(duration, runType)
        {
        }

        float TimerState() const {
            return timerState;
        }

        void SetTimerState(float value) {
            timerState = std::min(duration, std::max((float)0, value));
            SetIsFinished(timerState >= duration);
		}

		float Progress() const override
		{
            if (duration <= 0) { return 0; }
            return std::min(1.0f, TimerState() / duration);
		}

		void Reset() override
		{
            SomeTimed::Reset();

			SetTimerState(0);
		}

		/// <summary>
        /// Method, because overriden property is get only
        /// </summary>
        /// <param name="progress"></param>
		void SetProgress(float progress) {
			progress = std::min(1.0f, std::max((float)0, progress));
			SetTimerState(progress * duration);
		}

		void OnUpdate(TimeSlice time) override
		{
			if (IsFinished() || duration <= 0) { return; }

			auto delta = TimeDeltaFor(time);
			//Debug.Log(id + ": OnUpdate delta: " + delta.ToString());
			if (delta <= 0) { return; }

			SetTimerState(TimerState() + delta);
		}
    };
}

#endif
