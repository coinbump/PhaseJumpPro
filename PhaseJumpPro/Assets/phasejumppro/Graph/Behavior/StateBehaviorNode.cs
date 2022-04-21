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
    /// Transition to a state
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class StateBehaviorNode<T> : BehaviorNode
    {
        public T state;

        public StateBehaviorNode(T state)
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
                        return StateType.Success;
                    }
                }
            }

            return StateType.Fail;
        }
    }
}
