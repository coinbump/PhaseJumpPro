#pragma once

#include "DragModel.h"
#include "SomePosition.h"
#include "SomeUIEvent.h"
#include "UIWorldSystem.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    /// Abstract component for gestures/logic to handle dragging
    class DragHandler : public WorldComponent {
    public:
        using This = DragHandler;

        enum class StateType { Default, Drag };

        /// (Optional). If not null, drag the target object when this object is
        /// tapped Useful if the object with the collider has its own transform
        /// modifier which interferes with the drag or wants to be used as a
        /// drag handle for something else.
        WP<DragHandler> dragTarget;

        std::function<UIWorldSystem*()> uiSystemResolver;

        virtual UIWorldSystem* UISystem() const {
            GUARDR(uiSystemResolver, nullptr)
            return uiSystemResolver();
        }

    protected:
        StateType state;

        /// Position where the user clicked to initiate the drag
        WorldPosition dragStartInputPosition;

        /// Position where this object's transform was when the drag started
        WorldPosition dragStartPosition;

        void OnPointerDown(PointerDownUIEvent const& event);

    public:
        DragHandler();

        bool IsDragging() const;

        virtual void OnDragStart(WorldPosition inputPosition) {}

        virtual void OnDragUpdate(WorldPosition inputPosition) {}

        virtual void OnDragEnd();
        virtual void StartDrag(WorldPosition inputPosition);
    };
} // namespace PJ
