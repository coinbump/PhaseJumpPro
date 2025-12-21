#pragma once

#include "Collider2D.h"
#include "DragModel.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/15/24
 */
namespace PJ {
    /// Target that can accept drag items
    /// Requirement: node must have a collider for hit-testing
    class DropTarget : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = DropTarget;

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
        DropTarget();

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
            return "DropTarget";
        }

    protected:
        virtual void OnStateChange();

        // MARK: SomeWorldComponent

        void Awake() override;
    };
} // namespace PJ
