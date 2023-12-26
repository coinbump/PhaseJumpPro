using System;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/21/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Handles animation cycle logic for an animation
    /// </summary>
    public class AnimationCycleTimer : Updatable
    {
        public PJ.Timer timer;

        protected AnimationCycleType cycleType;
        protected AnimationCycleState cycleState;

        public AnimationCycleTimer(float duration, AnimationCycleType cycleType)
        {
            timer = new PJ.Timer(duration, SomeRunner.RunType.RunOnce);
            this.cycleType = cycleType;
        }

        public float Progress
        {
            get
            {
                switch (cycleState)
                {
                    case AnimationCycleState.Reverse:
                        return 1.0f - timer.Progress;
                    default:
                        return timer.Progress;
                }
            }
        }

        public bool IsFinished
        {
            get => timer.IsFinished;
        }

        public float CycleTime
        {
            get => timer.duration;
            set
            {
                timer.duration = value;
            }
        }

        public AnimationCycleState CycleState
        {
            get => cycleState;
            set
            {
                if (cycleState == value) { return; }
                cycleState = value;
            }
        }

        public AnimationCycleType CycleType
        {
            get => cycleType;
            set
            {
                if (cycleType == value) { return; }
                cycleType = value;
            }
        }

        public void SetProgress(float progress)
        {
            switch (cycleState)
            {
                case AnimationCycleState.Reverse:
                    timer.SetProgress(1.0f - progress);
                    break;
                default:
                    timer.SetProgress(progress);
                    break;
            }
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            if (timer.IsFinished) { return; }

            timer.OnUpdate(time);

            if (timer.IsFinished)
            {
                switch (cycleType)
                {
                    case AnimationCycleType.Once:
                        break;
                    case AnimationCycleType.Loop:
                        timer.Reset();
                        break;
                    case AnimationCycleType.PingPong:
                        cycleState = cycleState.Flip();
                        timer.Reset();
                        break;
                }
            }
        }
    }
}
