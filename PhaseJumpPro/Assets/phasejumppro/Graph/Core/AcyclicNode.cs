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
    namespace Graph
    {
        public class AcyclicNode<EdgeModel> : SomeGraphNode<EdgeModel>
        {
            public override void AddEdge(EdgeModel model, SomeGraphNode<EdgeModel> toNode)
            {
                // Acyclic graph nodes hold a strong reference to their next node
                AddEdgeInternal(model, new StrongReferenceType<SomeGraphNode<EdgeModel>>(toNode));
            }

            public AcyclicNode<EdgeModel> RootNode
            {
                /* HAS UNIT TEST */
                get
                {
                    var node = this;

                    var searchedNodes = new HashSet<AcyclicNode<EdgeModel>>();

                    while (node.FromNodes.Count > 0)
                    {
                        searchedNodes.Add(node);

                        foreach (HashedWeakReference<SomeGraphNode<EdgeModel>> weakFromNode in node.FromNodes)
                        {
                            if (weakFromNode.Reference.TryGetTarget(out SomeGraphNode<EdgeModel> target))
                            {
                                node = (AcyclicNode<EdgeModel>)target;
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

            public AcyclicNode<EdgeModel> Parent
            {
                /* HAS UNIT TEST */
                get
                {
                    foreach (HashedWeakReference<SomeGraphNode<EdgeModel>> weakFromNode in FromNodes)
                    {
                        if (weakFromNode.Reference.TryGetTarget(out SomeGraphNode<EdgeModel> target))
                        {
                            return (AcyclicNode<EdgeModel>)target;
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
        }
    }
}
