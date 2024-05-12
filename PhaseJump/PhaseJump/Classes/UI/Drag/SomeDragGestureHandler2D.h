#ifndef PJSOMEDRAGGESTUREHANDLER2D_H
#define PJSOMEDRAGGESTUREHANDLER2D_H

#include "SomeDragHandler.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ
{
    /// <summary>
    /// Updates with deltas of the drag while it is in progress
    /// </summary>
    class SomeDragGestureHandler2D : public SomeDragHandler
    {
    public:
        using Base = SomeDragHandler;

        void OnDragUpdate(WorldPosition inputPosition) override
        {
            auto delta = inputPosition - dragStartInputPosition;
            OnDragGestureUpdate(Vector2(delta.x, delta.y));
        }

        virtual void OnDragGestureUpdate(Vector2 delta)
        {
        }

        void OnDragEnd() override
        {
            Base::OnDragEnd();

            OnDragGestureUpdate(Vector2::zero);
        }
    };
}

#endif
