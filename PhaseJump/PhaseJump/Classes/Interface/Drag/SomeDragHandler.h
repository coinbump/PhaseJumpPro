#ifndef PJSOMEDRAGHANDLER_H
#define PJSOMEDRAGHANDLER_H

#include "WorldComponent.h"
#include "SomeUIEvent.h"
#include "SomePosition.h"
#include "UISystem.h"
#include "DragModel.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 5/10/23
 */
namespace PJ
{
    /// <summary>
    /// Abstract component for gestures/logic to handle dragging
    /// </summary>
    class SomeDragHandler : public WorldComponent, public SomePointerEventsResponder
    {
    public:
        enum class StateType
        {
            Default,
            Drag
        };

        /// <summary>
        /// (OPTIONAL). If not null, drag the target object when this object is tapped
        /// Useful if the object with the collider has its own transform modifier which interferes with the drag
        /// or wants to be used as a drag handle for something else.
        /// </summary>
        WP<SomeDragHandler> dragTarget;

        virtual SP<UISystem> UISystem() const
        {
            return UISystem::shared;
        }

    protected:
        StateType state;

        /// <summary>
        /// Position where the user clicked to initiate the drag
        /// </summary>
        WorldPosition dragStartInputPosition;

        /// <summary>
        /// Position where this object's transform was when the drag started
        /// </summary>
        WorldPosition dragStartPosition;

    public:
        virtual void OnDragStart(WorldPosition inputPosition) { }
        virtual void OnDragUpdate(WorldPosition inputPosition) = 0;

        bool IsDragging() const;
        virtual void OnDragEnd();
        virtual void StartDrag(WorldPosition inputPosition);
        void OnPointerDownEvent(PointerDownUIEvent<LocalPosition> event) override;
    };
}

#endif
