using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    /// <summary>
    /// Transition to a state, and hold on that state for N seconds
    /// </summary>
    public class TimedStateBehaviorNode<T> : TimedBehaviorNode
    {
        public T state;

        public TimedStateBehaviorNode(T state, float duration) : base(duration)
        {
            this.state = state;
        }

        protected override StateType Evaluate()
        {
            if (null != owner)
            {
                if (owner.TryGetTarget(out object target))
                {
                    var statable = target as Statable<T>;
                    if (null != statable)
                    {
                        statable.State = state;
                        return base.Evaluate();
                    }
                }
            }

            return StateType.Fail;
        }
    }
}
