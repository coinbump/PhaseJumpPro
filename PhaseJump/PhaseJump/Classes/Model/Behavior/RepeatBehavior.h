#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/16/24
 */
namespace PJ {
    /// Repeats a behavior N times
    class RepeatBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = RepeatBehavior;

        using RepeatValueFunc = std::function<int()>;

    protected:
        int repeats{};
        int repeatMax{};

    public:
        /// Returns the number of repeats
        RepeatValueFunc repeatValueFunc;

        RepeatBehavior(RepeatValueFunc repeatValueFunc);
        RepeatBehavior(int repeat);

        int RepeatMax() const {
            return repeatMax;
        }

        int RepeatCount() const {
            return repeatMax - repeats;
        }

        // MARK: BehaviorNode

        BehaviorNode* Target() override;
        void OnTargetFinish(BehaviorNode& target) override;
        void OnRun() override;
    };
} // namespace PJ
