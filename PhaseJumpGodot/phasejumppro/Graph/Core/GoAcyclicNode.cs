using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 3/6/22
 */
namespace PJ
{
    /// <summary>
    /// Extends Graph.Node with common Go-type functionality
    /// </summary>
    /// <typeparam name="EdgeModel"></typeparam>
    public class GoAcyclicGraphNode<EdgeModel> : AcyclicGraphNode<EdgeModel>
    {
        public virtual bool IsValidTarget(AcyclicGraphNode<EdgeModel> node) => false;

        /// <summary>
        /// Apply an operation to the target node (optional)
        /// </summary>
        /// <param name="node"></param>
        public virtual void Target(AcyclicGraphNode<EdgeModel> node) { }

        /// <summary>
        /// Pick a child, or the next node in a sequence (optional)
        /// </summary>
        /// <returns>
        /// Returns the next node in the sequence
        /// </returns>
        public virtual AcyclicGraphNode<EdgeModel> SelectNext() => null;
    }
}
