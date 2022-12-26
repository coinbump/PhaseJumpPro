using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple, with unit tests
 * Code review: 1/7/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Selects children in sequence
        /// </summary>
        /// <typeparam name="EdgeModel"></typeparam>
        public class SerialSelectorNode<EdgeModel> : GoAcyclicGraphNode<EdgeModel> where EdgeModel : new()
        {
            protected int currentIndex = 0;

            public SerialSelectorNode()
            {
            }

            public SerialSelectorNode(List<AcyclicGraphNode<EdgeModel>> nodes)
            {
                foreach (AcyclicGraphNode<EdgeModel> node in nodes)
                {
                    AddEdge(new(), node);
                }
            }

            public override AcyclicGraphNode<EdgeModel> SelectNext()
            {
                if (edges.Count == 0) { return null; }

                var selectIndex = currentIndex;
                if (selectIndex >= edges.Count)
                {
                    selectIndex = 0;
                }

                currentIndex += 1;
                if (currentIndex >= edges.Count)
                {
                    currentIndex = 0;
                }

                return (AcyclicGraphNode<EdgeModel>)edges[selectIndex].toNode.Value;
            }
        }
    }
}
