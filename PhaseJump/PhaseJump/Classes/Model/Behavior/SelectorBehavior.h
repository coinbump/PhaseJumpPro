#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/19/24
 */
namespace PJ {
    /// Runs child behaviors in order until one succeeds, then reports success
    /// If all child behaviors fail, reports failure
    class SelectorBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = SelectorBehavior;

    protected:
        size_t targetIndex{};

    public:
        // MARK: BehaviorNode

        BehaviorNode* Target() override;
        void OnTargetFinish(BehaviorNode& target) override;
        void OnRun() override;
    };
} // namespace PJ
