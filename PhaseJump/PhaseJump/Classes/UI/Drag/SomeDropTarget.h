#pragma once

#include "DragModel.h"
#include "SomeCollider2D.h"
#include "WorldComponent.h"

/*
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    /// Target that can accept drag items
    /// Requirement: node must have a collider for hit-testing
    class SomeDropTarget : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = SomeDropTarget;

        using CanAcceptDragFunc = std::function<bool(This&, DragModel const&)>;
        using OnStateChangeFunc = std::function<void(This&)>;
        using OnDropFunc = std::function<void(This&, DragModel const&)>;

        enum class StateType {
            /// Default state
            Default,

            /// A drag is over this drop target and can be dropped here
            DragAccept,

            /// A drag is over this drop target, but can't be dropped here
            DragReject
        };

        // FUTURE: rethink how effects are triggered by drop states

        CanAcceptDragFunc canAcceptDragFunc;
        OnStateChangeFunc onStateChangeFunc;
        OnDropFunc onDropFunc;

    protected:
        StateType state = StateType::Default;

    public:
        SomeDropTarget();

        StateType State() const {
            return state;
        }

        void SetState(StateType value);

        virtual bool CanAcceptDrag(DragModel const& dragModel);
        virtual void OnDragEnter(DragModel const& dragModel);
        virtual void OnDragExit();
        virtual void OnDrop(DragModel const& dragModel);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SomeDropTarget";
        }

    protected:
        virtual void OnStateChange();

        // MARK: SomeWorldComponent

        void Awake() override;
    };
} // namespace PJ
