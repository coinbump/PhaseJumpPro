#pragma once

#include "DragHandler.h"

/*
 RATING: 5+ stars
 Tested and works
 CODE REVIEW: 4/16/26
 */
namespace PJ {
    /**
     Attach to objects that can be dragged
     Requirements:
     - Attach a raycaster to the camera
     - Create a UI system object
     - Add DragHandler2D to objects you want to be draggable
     - Attach a collider to the draggable object
     */
    class DragHandler2D : public DragHandler {
    public:
        enum class DropType {
            /// The drag ended
            End,

            /// The drag was cancelled
            Cancel
        };

        using Base = DragHandler;
        using This = DragHandler2D;

        using OnDropFunc = std::function<void(This&, DropType)>;

        OnDropFunc onDropFunc;

        void Drop(DropType dropType);

        This& SetOnDropFunc(OnDropFunc onDropFunc) {
            this->onDropFunc = onDropFunc;
            return *this;
        }

        /// Sets the behavior for OnDrop to snap back to the original position before the drag
        /// started
        void InOnDropSnapBack();

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "DragHandler2D";
        }

    protected:
        // MARK: DragHandler

        void OnDragUpdate(WorldPosition position) override;
        void OnDragEnd(WorldPosition position) override;
        void OnDragCancel() override;
    };
} // namespace PJ
