using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    /// <summary>
    /// Randomize node order before evaluating
    /// </summary>
    public class RandomSelectorBehaviorNode : SelectorBehaviorNode
    {
        protected override StateType Evaluate()
        {
            edges.Shuffle();
            return base.Evaluate();
        }
    }
}
