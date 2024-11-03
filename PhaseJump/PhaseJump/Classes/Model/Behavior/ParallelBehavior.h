#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/1/24
 */
namespace PJ {
    /// Runs child behaviors in parallel until N behaviors succeed, then reports success
    /// If N child behaviors fail, reports failure
    class ParallelBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = ParallelBehavior;

    public:
        /// Indicates that all nodes must fail/succeed
        static int constexpr AllThreshold = -1;

        /// # of child nodes that need to succeed for parallel node to succeed
        int successThreshold = AllThreshold;

        /// # of child nodes that need to fail for parallel node to fail
        int failureThreshold = AllThreshold;

        ParallelBehavior(int successThreshold = AllThreshold, int failureThreshold = AllThreshold);

        // MARK: BehaviorNode

        VectorList<BehaviorNode*> Targets() override;
        void OnTargetFinish(BehaviorNode& target) override;
    };
} // namespace PJ
