#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/16/24
 */
namespace PJ {
    /// Run the target behavior after a specified delay
    class DelayBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = DelayBehavior;
        using DelayValueFunc = std::function<float()>;

    protected:
        /// Seconds left in delay
        float delay{};

    public:
        /// Returns delay value in seconds
        DelayValueFunc delayValueFunc;

        DelayBehavior(std::function<float()> delayValueFunc);
        DelayBehavior(float delay);

        // MARK: BehaviorNode

        BehaviorNode* Target() override;
        void OnRun() override;
    };
} // namespace PJ
