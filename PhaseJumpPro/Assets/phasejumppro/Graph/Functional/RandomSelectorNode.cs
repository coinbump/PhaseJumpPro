/*
 * RATING: 5 stars
 * Simple, with unit tests
 * Code review: 1/7/21
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Selects a random child (unweighted)
        /// </summary>
        /// <typeparam name="EdgeModel"></typeparam>
        public class RandomSelectorNode<EdgeModel> : GoAcyclicGraphNode<EdgeModel>
        {
            public override AcyclicGraphNode<EdgeModel> SelectNext()
            {
                if (edges.Count == 0) { return null; }

                var edge = RandomUtils.ChooseFrom(edges);
                return (AcyclicGraphNode<EdgeModel>)edge.toNode.Value;
            }
        }
    }
}
