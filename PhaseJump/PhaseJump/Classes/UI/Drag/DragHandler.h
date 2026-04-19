#pragma once

#include "DragModel.h"
#include "SomePosition.h"
#include "SomeUIEvent.h"
#include "WorldComponent.h"

/*
 RATING: 5+ stars
 Tested and works
 CODE REVIEW: 4/16/26
 */
namespace PJ {
    class UIWorldSystem;
    class PointerDownUIEvent;

    enum class GestureStateType {
        /// Default state
        Default,

        /// Gesture is progress
        Began,
    };

    /// Abstract component for gestures/logic to handle dragging
    /// Requirements:
    /// - UIWorldSystem to track drag events
    class DragHandler : public WorldComponent {
    public:
        friend class UIWorldSystem;

        using This = DragHandler;
        using StateType = GestureStateType;
        using OnDragUpdateFunc = std::function<void(This&, WorldPosition)>;

        OnDragUpdateFunc onDragUpdateFunc;

        /// If returns a drag handler, drag the target object when this object is
        /// tapped Useful if the object with the collider has its own transform
        /// modifier which interferes with the drag or wants to be used as a
        /// drag handle for something else.
        std::function<DragHandler*()> dragTargetResolver;

        /// Returns a UI system for drag handling
        std::function<UIWorldSystem*()> uiSystemResolver;

        This& SetOnDragUpdateFunc(OnDragUpdateFunc onDragUpdateFunc) {
            this->onDragUpdateFunc = onDragUpdateFunc;
            return *this;
        }

        virtual UIWorldSystem* UISystem() const {
            GUARDR(uiSystemResolver, nullptr)
            return uiSystemResolver();
        }

    protected:
        StateType state{ StateType::Default };

        /// Position where the user clicked to initiate the drag
        WorldPosition dragStartPosition;

        /// Position where this node's transform was when the drag started
        WorldPosition dragNodeStartPosition;

        void OnPointerDown(PointerDownUIEvent const& event);

    public:
        DragHandler();

        bool IsDragging() const;

        virtual void StartDrag(WorldPosition position);
        virtual void Cancel();

    protected:
        virtual void OnDragStart(WorldPosition position) {};

        virtual void OnDragUpdate(WorldPosition position) {
            GUARD(onDragUpdateFunc)
            onDragUpdateFunc(*this, position);
        };

        virtual void OnDragEnd(WorldPosition position);
        virtual void OnDragCancel();
    };
} // namespace PJ
