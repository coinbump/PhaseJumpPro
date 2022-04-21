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
    /// Evaluates each child node in order. If a child fails, the next node will be evaluated
    /// </summary>
    public class SequenceBehaviorNode : BehaviorNode
    {
        /// <summary>
        /// If true, sequence repeats
        /// </summary>
        public bool isRepeating = false;

        protected int childIndex;

        protected int ChildIndex
        {
            get => childIndex;
            set
            {
                if (isRepeating)
                {
                    childIndex = value % Edges.Count;
                }
                else
                {
                    childIndex = value;
                }
            }
        }

        public SequenceBehaviorNode(bool isRepeating)
        {
            this.isRepeating = isRepeating;
        }

        protected override StateType Evaluate()
        {
            for (; childIndex < Edges.Count; )
            {
                var child = (BehaviorNode)Edges[childIndex].toNode.Value;
                var childState = child.Run();

                ChildIndex++;

                if (childIndex == Edges.Count)
                {
                    return childState;
                }
                else
                {
                    // If child fails, try the next one until we run out
                    switch (childState) {
                        case StateType.Running:
                        case StateType.Success:
                            return childState;
                    }
                }
            }

            return StateType.Fail;
        }
    }
}
