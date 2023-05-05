#ifndef PJGOACYCLICGRAPHNODE_H
#define PJGOACYCLICGRAPHNODE_H

#include "AcyclicNode.h"

/*
 RATING: 5 stars
 Convenience pattern
 CODE REVIEW: 12/11/22
 */
namespace PJ
{
    /// <summary>
    /// Extends Graph.Node with common Go-type functionality
    /// </summary>
    class GoAcyclicGraphNode<EdgeModel> : public AcyclicNode<EdgeModel>
    {
    public:
        using Base = AcyclicNode<EdgeModel>;

        virtual bool IsValidTarget(AcyclicNode<EdgeModel> node) {
            return false;
        }

        /// <summary>
        /// Apply an operation to the target node (optional)
        /// </summary>
        virtual void Target(AcyclicNode<EdgeModel> node) { }

        /// <summary>
        /// Pick a child, or the next node in a sequence (optional)
        /// </summary>
        /// <returns>
        /// Returns the next node in the sequence
        /// </returns>
        virtual SP<Base> SelectNext() {
            return SP<Base>();
        }
    };
}

#endif
