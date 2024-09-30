#ifndef PJGOACYCLICGRAPHNODE_H
#define PJGOACYCLICGRAPHNODE_H

#include "AcyclicNode.h"

/*
 RATING: 5 stars
 Convenience pattern
 CODE REVIEW: 5/12/24
 */
namespace PJ {
    /// Extends Graph.Node with common Go-type functionality
    class GoAcyclicGraphNode<EdgeCore> : public AcyclicNode<EdgeCore> {
    public:
        using Base = AcyclicNode<EdgeCore>;

        virtual bool IsValidTarget(AcyclicNode<EdgeCore> node) {
            return false;
        }

        /// Apply an operation to the target node (optional)
        virtual void Target(AcyclicNode<EdgeCore> node) {}

        /// Pick a child, or the next node in a sequence (optional)
        /// <returns>
        /// @return Returns the next node in the sequence
        /// </returns>
        virtual SP<Base> SelectNext() {
            return SP<Base>();
        }
    };
} // namespace PJ

#endif
