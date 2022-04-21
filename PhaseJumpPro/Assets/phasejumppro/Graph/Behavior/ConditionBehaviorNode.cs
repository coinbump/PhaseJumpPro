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
    /// Evaluates a condition and if true, continues to first child node
    /// </summary>
    public abstract class ConditionBehaviorNode : BehaviorNode
    {
        public abstract bool EvaluateCondition();

        protected override StateType Evaluate()
        {
            // A condition node with no children is a no-op
            if (Edges.Count <= 0)
            {
                return StateType.Fail;
            }

            if (EvaluateCondition())
            {
                var child = (BehaviorNode)Edges[0].toNode.Value;
                return child.Run();
            }

            return StateType.Fail;
        }
    }
}
