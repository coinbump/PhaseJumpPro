#ifndef PJTIMER_H_
#define PJTIMER_H_

#include <algorithm>
#include <string>

/*
 * RATING: 4 stars
 * Simple timer (needs Unit Tests ported from C#)
 * CODE REVIEW: 11/5/22
 */
namespace PJ {
	/// <summary>
    /// Keeps track of time for N seconds duration, and then triggers OnFinish
    /// </summary>
	class Timer : SomeTimed
	{
    public:
		/// <summary>
        /// Optional id for hashing, debug logs
        /// </summary>
		std::string id;

    protected:
		float timerState;  // Time state of the timer

    public:
        float TimerState() {
            return timerState;
        }

        void SetTimerState(float value) {
            timerState = std::min(duration, std::max((float)0, value));
            SetIsFinished(timerState >= duration);
		}

		float Progress() override
		{
            if (duration <= 0) { return 0; }
            return std::min(1.0f, TimerState() / duration);
		}

		Timer(float duration, RunType runType)
        : SomeTimed(duration, runType)
		{
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
