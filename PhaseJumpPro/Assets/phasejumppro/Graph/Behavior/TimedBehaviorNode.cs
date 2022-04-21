using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    /// <summary>
    /// Behavior runs for N seconds
    /// Example: entity searches for food for N seconds, then gives up
    /// Sequence: State(food) -> Timed(10) -> State(give up)
    /// </summary>
    public class TimedBehaviorNode : BehaviorNode
    {
        Timer timer;

        public float Duration
        {
            get
            {
                return timer != null ? timer.duration : 0;
            }
            set
            {
                if (timer == null) { timer = new Timer(value, SomeTimed.RunType.RunOnce); }
                timer.duration = value;
            }
        }

        public TimedBehaviorNode(float duration)
        {
            timer = new Timer(duration, SomeTimed.RunType.RunOnce);
        }

        public override void OnUpdateNode(TimeSlice time)
        {
            base.OnUpdateNode(time);

            if (IsRunning && null != timer && !timer.IsFinished)
            {
                timer.OnUpdate(time);
                if (timer.IsFinished)
                {
                    stateMachine.State = StateType.Finish;
                }
            }
        }

        protected override StateType Evaluate()
        {
            if (null == timer) { return StateType.Fail; }

            timer.Reset();

            if (timer.duration > 0)
            {
                return StateType.Running;
            }
            else
            {
                return StateType.Fail;
            }
        }
    }
}
