#pragma once

#include "DiagnoseModel.h"
#include "Runner.h"
#include "StateMachine.h"
#include "TreeNode.h"
#include "Updatable.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/19/24
 */
namespace PJ {
    enum class BehaviorState {
        /// Initial state for behavior node
        Initial,

        /// Behavior is executing
        Run,

        /// Behavior has finished with success
        Success,

        /// Behavior has finished with failure
        Failure
    };

    /**
     Node in a behavior tree

     Reference:
     https://docs.nvidia.com/holoscan/sdk-user-guide/gxf/doc/behavior_tree/behavior_trees.html
     */
    class BehaviorNode : public Treeable<BehaviorNode>, public Updatable {
    public:
        // FUTURE: add ParallelBehavior if needed
        using Base = Updatable;
        using This = BehaviorNode;
        using Tree = TreeNode<This, UP<This>>;

        using Func = std::function<void(This&)>;
        using OnTargetFinishFunc = std::function<void(This&, This& target)>;

    protected:
        StateMachine<BehaviorState> states;

        /// Called when this node starts running
        virtual void OnRun();

    public:
        /// Used to enable extra logging for this object
        DiagnoseModel _diagnose;

        Tree tree;

        /// Behavior name. Used for logging
        String name;

        /// State to transition to when this behavior finishes
        BehaviorState finishState = BehaviorState::Success;

        /// Called when this behavior starts running
        Func onRunFunc;

        /// Called when the target child behavior finishes
        OnTargetFinishFunc onTargetFinishFunc;

        /// Called when all children have finished running
        Func onAllChildrenFinishFunc;

        BehaviorNode(String name = {}, RunType runType = RunType::Once);

        void Add(UP<This>& value) {
            tree.Add(value);
        }

        BehaviorState State() const {
            return states.State();
        }

        bool IsRunning() const {
            return states.State() == BehaviorState::Run;
        }

        void Reset() {
            states.SetState(BehaviorState::Initial);
        }

        int ChildSuccessCount() const;
        int ChildFailureCount() const;

        /// Called when the target node finishes running
        virtual void OnTargetFinish(BehaviorNode& target);

        /// @return Returns the child node to target
        virtual BehaviorNode* Target();

        /// @return Returns a list of child nodes to target
        virtual VectorList<BehaviorNode*> Targets();

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;

        // MARK: Treeable

        This* Parent() const override {
            return tree.Parent();
        }

        void SetParent(This* value) override {
            tree.SetParent(value);
        }
    };
} // namespace PJ
