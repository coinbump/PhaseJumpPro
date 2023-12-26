using System;
using System.Collections.Generic;

/*
 * RATING: 4 stars
 * Standard model, with unit tests
 * 
 * LIMITATION: graphs with cycles will create strong reference cycles
 * This should not be used to create graphs with cycles, unless you deconstruct them manually
 * 
 * CODE REVIEW: 3/8/22
 */
namespace PJ
{
    public class AcyclicGraphNode<EdgeModel> : SomeGraphNode<EdgeModel>
    {
        public override void AddEdge(EdgeModel model, SomeGraphNode<EdgeModel> toNode)
        {
            // Acyclic graph nodes hold a strong reference to their next node
            AddEdgeInternal(model, new StrongReference<SomeGraphNode<EdgeModel>>(toNode));
        }

        public AcyclicGraphNode<EdgeModel> Root
        {
            /* HAS UNIT TEST */
            get
            {
                var node = this;

                var searchedNodes = new HashSet<AcyclicGraphNode<EdgeModel>>();

                while (node.FromNodes.Count > 0)
                {
                    searchedNodes.Add(node);

                    foreach (HashedWeakReference<SomeGraphNode<EdgeModel>> weakFromNode in node.FromNodes)
                    {
                        if (weakFromNode.Reference.TryGetTarget(out SomeGraphNode<EdgeModel> target))
                        {
                            node = (AcyclicGraphNode<EdgeModel>)target;
                            break;
                        }
                    }

                    // Graph shouldn't be cyclic, but prevent the edge case of an infinite loop
                    if (searchedNodes.Contains(node))
                    {
                        return null;
                    }
                }

                return node;
            }
        }

        public AcyclicGraphNode<EdgeModel> Parent
        {
            /* HAS UNIT TEST */
            get
            {
                foreach (HashedWeakReference<SomeGraphNode<EdgeModel>> weakFromNode in FromNodes)
                {
                    if (weakFromNode.Reference.TryGetTarget(out SomeGraphNode<EdgeModel> target))
                    {
                        return (AcyclicGraphNode<EdgeModel>)target;
                    }
                }

                return null;
            }
        }

        public bool IsRootNode
        {
            /* HAS UNIT TEST */
            get
            {
                return FromNodes.Count <= 0;
            }
        }

        /// <summary>
        /// Only the root node should have OnUpdate called, every other node should override OnUpdateNode instead
        /// </summary>
        public override void OnUpdate(TimeSlice time)
        {
            if (IsRootNode)
            {
                var nodes = CollectGraph();
                foreach (SomeGraphNode<EdgeModel> node in nodes)
                {
                    if (node == this) { continue; }
                    node.OnUpdate(time);
                }
            }
        }
    }
}
