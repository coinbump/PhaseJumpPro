#ifndef PJDRAGHANDLER2D_H
#define PJDRAGHANDLER2D_H

#include "SomeDragHandler.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 5/10/23
 */
namespace PJ
{
    /// <summary>
    /// Attach to objects that can be dragged
    /// </summary>
    /// REQUIREMENTS:
    /// - Attach a raycaster to the camera
    /// - Create an UISystem object
    /// - Add DragHandler2D to objects you want to be draggable
    /// - Attach a collider to the draggable object
    class DragHandler2D : public SomeDragHandler
    {
    public:
        using Base = SomeDragHandler;

        /// <summary>
        /// What to do when the object is dropped
        /// </summary>
        enum class DropType
        {
            Stay,    // Stay at position where it was dropped
            SnapBack // Snap back to original position
        };

        DropType dropType = DropType::Stay;

        virtual void Drop();
        void OnDragUpdate(WorldPosition inputPosition) override;
        void OnDragEnd() override;
    };
}

#endif
