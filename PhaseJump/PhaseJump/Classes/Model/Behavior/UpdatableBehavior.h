#pragma once

#include "BehaviorNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/19/24
 */
namespace PJ {
    /// Runs a dynamically created updatable until it is finished
    /// Example: run an animation timeline
    class UpdatableBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = UpdatableBehavior;

        using BuildFunc = std::function<UP<SomeUpdatable>(This&)>;

    protected:
        /// Behavior runs the updatable until it is finished
        UP<SomeUpdatable> childUpdatable;

    public:
        /// Called to build the updatable dynamically
        BuildFunc buildFunc;

        UpdatableBehavior(BuildFunc buildFunc);

        SomeUpdatable* GetChildUpdatable() const {
            return childUpdatable.get();
        }

        // MARK: BehaviorNode

        void OnRun() override;
    };
} // namespace PJ
