using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Simple abstract class
 * CODE REVIEW: 3/6/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Extends Graph.Node with common Go-type functionality
        /// </summary>
        /// <typeparam name="EdgeModel"></typeparam>
        public class GoNode<EdgeModel> : Node<EdgeModel>
        {
            /// <summary>
            /// Kickstart some process (optional)
            /// </summary>
            public virtual void Go() { }

            public virtual bool IsValidTarget(Node<EdgeModel> node) => false;

            /// <summary>
            /// Apply an operation to the target node (optional)
            /// </summary>
            /// <param name="node"></param>
            public virtual void Target(Node<EdgeModel> node) { }

            /// <summary>
            /// Pick a child, or the next node in a sequence (optional)
            /// </summary>
            /// <returns>
            /// Returns the next node in the sequence
            /// </returns>
            public virtual Node<EdgeModel> SelectNext() => null;
        }
    }
}
