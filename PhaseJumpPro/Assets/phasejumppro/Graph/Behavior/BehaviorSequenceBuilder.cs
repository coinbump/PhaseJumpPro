using System;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    /// <summary>
    /// Utility to speed up building behavior sequences
    /// </summary>
    public class BehaviorSequenceBuilder<StateType>
    {
        public SequenceBehaviorNode sequenceNode;

        public BehaviorSequenceBuilder(bool isRepeating)
        {
            sequenceNode = new SequenceBehaviorNode(isRepeating);
        }

        public BehaviorSequenceBuilder<StateType> Wait(float duration)
        {
            var node = new TimedBehaviorNode(duration);
            sequenceNode.AddChild(node);
            return this;
        }

        public BehaviorSequenceBuilder<StateType> State(StateType state, float duration = 0)
        {
            if (duration > 0)
            {
                return StateTimed(state, duration);
            }

            var node = new StateBehaviorNode<StateType>(state);
            sequenceNode.AddChild(node);

            return this;
        }

        public BehaviorSequenceBuilder<StateType> StateTimed(StateType state, float duration)
        {
            var node = new TimedStateBehaviorNode<StateType>(state, duration);
            sequenceNode.AddChild(node);

            return this;
        }
    }
}
