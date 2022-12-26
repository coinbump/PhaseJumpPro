/*
 * RATING: 5 stars
 * Has unit tests (via Timer)
 * CODE REVIEW: 12/19/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Runs then decides what to do when finished running
    /// Example: run a timer, every time it finishes, fire an event, reset timer
    /// </summary>
    public abstract class SomeRunner : Updatable
    {
        public enum RunType
        {
            // Destroy when finished
            RunOnce,

            // Repeats (example: timer that does an update every N seconds)
            KeepRunning
        }

        public RunType runType;

        /// <summary>
        /// If true, we are finished running
        /// </summary>
        private bool isFinished;

        public SomeRunner(RunType runType)
        {
            this.runType = runType;
        }

        public bool IsFinished
        {
            get => isFinished;
            set
            {
                if (value != isFinished)
                {
                    isFinished = value;

                    if (isFinished)
                    {
                        OnFinish();

                        switch (runType)
                        {
                            case RunType.KeepRunning:
                                Reset();
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }

        public virtual void Reset()
        {
            isFinished = false;
        }

        protected virtual void OnFinish() { }
        public virtual void Go() { }

        public virtual void OnUpdate(TimeSlice time) { }
    }
}
