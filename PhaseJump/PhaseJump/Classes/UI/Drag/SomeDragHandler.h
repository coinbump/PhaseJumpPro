#ifndef PJSOMEDRAGHANDLER_H
#define PJSOMEDRAGHANDLER_H

#include "DragModel.h"
#include "SomePosition.h"
#include "SomeUIEvent.h"
#include "UISystem.h"
#include "WorldComponent.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 5/10/23
 */
namespace PJ {
    /// Abstract component for gestures/logic to handle dragging
    class SomeDragHandler : public WorldComponent<>, public SomePointerUIEventsResponder {
    public:
        enum class StateType { Default, Drag };

        /// (OPTIONAL). If not null, drag the target object when this object is
        /// tapped Useful if the object with the collider has its own transform
        /// modifier which interferes with the drag or wants to be used as a
        /// drag handle for something else.
        WP<SomeDragHandler> dragTarget;

        std::function<UISystem*()> uiSystemResolver = []() { return UISystem::shared; };

        virtual UISystem* UISystem() const {
            GUARDR(uiSystemResolver, nullptr)
            return uiSystemResolver();
        }

    protected:
        StateType state;

        /// Position where the user clicked to initiate the drag
        WorldPosition dragStartInputPosition;

        /// Position where this object's transform was when the drag started
        WorldPosition dragStartPosition;

    public:
        virtual void OnDragStart(WorldPosition inputPosition) {}

        virtual void OnDragUpdate(WorldPosition inputPosition) = 0;

        bool IsDragging() const;
        virtual void OnDragEnd();
        virtual void StartDrag(WorldPosition inputPosition);
        void OnPointerDown(PointerDownUIEvent event) override;
    };
} // namespace PJ

#endif
