#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/16/24
 */
namespace PJ {
    /// Chooses a child behavior to target based on a func
    class SwitchBehavior : public BehaviorNode {
    protected:
        BehaviorNode* target{};

    public:
        using Base = BehaviorNode;
        using This = SwitchBehavior;

        using ChooseFunc = std::function<size_t(This&)>;

        ChooseFunc chooseFunc;

        SwitchBehavior(ChooseFunc chooseFunc);

        // MARK: BehaviorNode

        BehaviorNode* Target() override;
        void OnRun() override;
        void OnTargetFinish(BehaviorNode& target) override;
    };
} // namespace PJ
