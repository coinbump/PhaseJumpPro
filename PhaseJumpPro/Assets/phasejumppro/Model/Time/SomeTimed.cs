/*
 * RATING: 5 stars
 * Has unit tests (via Timer)
 * CODE REVIEW: 12/19/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Any object that relies on timers to perform tasks
    /// 
    /// Supports an internal time scale for animating an object's time
    /// </summary>
    public abstract class SomeTimed : SomeRunner
    {
        public abstract float Progress { get; }

        public float duration;

        /// <summary>
        /// Used to speed up/slow down timer
        /// </summary>
        public float timeScale = 1.0f;

        /// <summary>
        /// If true, timer will stop receiving time events
        /// </summary>
        protected bool isPaused;

        public SomeTimed(float duration, SomeRunner.RunType runType) : base(runType)
        {
            this.duration = duration;
        }

        public bool IsRunning
        {
            get => !isPaused;
            set
            {
                isPaused = !value;
            }
        }

        public void Pause(bool pause)
        {
            IsRunning = !pause;
        }

        public override void Reset()
        {
            base.Reset();
            Pause(false);
        }

        public float TimeDeltaFor(TimeSlice time)
        {
            // Paused timer. No time events
            if (isPaused)
            {
                return 0;
            }

            var result = time.delta;
            result *= timeScale;
            return result;
        }
    }
}
