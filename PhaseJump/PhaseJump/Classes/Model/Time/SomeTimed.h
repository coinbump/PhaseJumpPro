#ifndef PJSOMETIMED_H_
#define PJSOMETIMED_H_

#include "SomeRunner.h"

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
	class SomeTimed : public SomeRunner {
    protected:
        /// <summary>
        /// If true, timer will stop receiving time events
        /// </summary>
        bool isPaused = false;

    public:
        /// <summary>
        /// Used to speed up/slow down timer
        /// </summary>
        float timeScale = 1.0f;

        float duration;

        SomeTimed(float duration, RunType runType) : SomeRunner(runType), duration(duration) {
        }
        virtual ~SomeTimed() {}

        virtual float Progress() const = 0;

		bool IsRunning() const
        {
            return !isPaused;
        }

        void SetIsRunning(bool value) {
            isPaused = !value;
		}

		void Pause(bool pause)
		{
			SetIsRunning(!pause);
		}

		void Reset() override
        {
            SomeRunner::Reset();
			Pause(false);
		}

		float TimeDeltaFor(TimeSlice time)
		{
			// Paused timer. No time events
			if (isPaused) {
				return 0;
			}

			auto result = time.delta;
			result *= timeScale;
			return result;
		}

		virtual void Go() {}
    };
}

#endif
