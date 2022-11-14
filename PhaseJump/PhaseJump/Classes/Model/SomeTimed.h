#ifndef PJSOMETIMED_H_
#define PJSOMETIMED_H_

/*
 * RATING: 5 stars
 * Has unit tests (via Timer)
 * CODE REVIEW: 4/14/22
 */
namespace PJ {
	/// <summary>
	/// Any object that relies on timers to perform tasks
	///
	/// Supports an internal time scale for animating an object's time
	/// </summary>
	class SomeTimed : Updatable {
    public:
        virtual ~SomeTimed() {}

        virtual float Progress() = 0;

		enum class RunType {
            // Destroy when finished
			RunOnce,

            // Repeats (example: timer that does an update every N seconds)
			KeepRunning
        };

		float duration;
		RunType runType;

		/// <summary>
		/// Used to speed up/slow down timer
		/// </summary>
		float timeScale = 1.0f;

    protected:
		/// <summary>
        /// If true, timer has reached the end of its duration
        /// </summary>
		bool isFinished;

		/// <summary>
        /// If true, timer will stop receiving time events
        /// </summary>
		bool isPaused;

    public:
        SomeTimed(float duration, RunType runType) : duration(duration), runType(runType) {
		}

		bool IsFinished()
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

		bool IsRunning()
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

		virtual void Reset()
        {
			isFinished = false;
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

		virtual void OnUpdate(TimeSlice time) {}
		virtual void Go() {}

    protected:
        virtual void OnFinish() {}
    };
}

#endif