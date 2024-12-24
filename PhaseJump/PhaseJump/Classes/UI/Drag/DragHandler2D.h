#pragma once

#include "SomeDragHandler.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/22/24
 */
namespace PJ {
    /**
     Attach to objects that can be dragged
     Requirements:
     - Attach a raycaster to the camera
     - Create an UISystem object
     - Add DragHandler2D to objects you want to be draggable
     - Attach a collider to the draggable object
     */
    class DragHandler2D : public SomeDragHandler {
    public:
        using Base = SomeDragHandler;
        using This = DragHandler2D;

        using OnDragUpdateFunc = std::function<void(This&)>;
        using OnDropFunc = std::function<void(This&)>;

        OnDragUpdateFunc onDragUpdateFunc;
        OnDropFunc onDropFunc;

        virtual void Drop();

        DragHandler2D& SetOnDragUpdateFunc(OnDragUpdateFunc onDragUpdateFunc) {
            this->onDragUpdateFunc = onDragUpdateFunc;
            return *this;
        }

        /// Sets the behavior for OnDrop to snap back to the original position before the drag
        /// started
        void InOnDropSnapBack();

        // MARK: SomeDragHandler

        void OnDragUpdate(WorldPosition inputPosition) override;
        void OnDragEnd() override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "DragHandler2D";
        }
    };
} // namespace PJ
