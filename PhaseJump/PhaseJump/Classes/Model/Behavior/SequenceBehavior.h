#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/16/24
 */
namespace PJ {
    /// Runs child behaviors in sequence.
    /// Fails if a child behavior fails. Succeeds if all child behaviors succeed
    class SequenceBehavior : public BehaviorNode {
    protected:
        size_t targetIndex{};

    public:
        using Base = BehaviorNode;
        using This = SequenceBehavior;

        size_t TargetIndex() const {
            return targetIndex;
        }

        // MARK: BehaviorNode

        BehaviorNode* Target() override;
        void OnTargetFinish(BehaviorNode& target) override;
        void OnRun() override;
    };
} // namespace PJ
