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
    /// Selects the first child that is successful or running
    /// </summary>
    public class SelectorBehaviorNode : BehaviorNode
    {
        protected override StateType Evaluate()
        {
            for (int childIndex = 0; childIndex < Edges.Count; childIndex++)
            {
                var child = (BehaviorNode)Edges[childIndex].toNode.Value;
                var childState = child.Run();

                switch (childState)
                {
                    // Keep looking for a child can run
                    case StateType.Fail:
                        break;
                    default:
                        return childState;
                }
            }

            return StateType.Fail;
        }
    }
}
