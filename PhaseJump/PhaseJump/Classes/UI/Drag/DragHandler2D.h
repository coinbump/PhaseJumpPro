#pragma once

#include "SomeDragHandler.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 5/10/23
 */
namespace PJ {
    /// Attach to objects that can be dragged
    /// REQUIREMENTS:
    /// - Attach a raycaster to the camera
    /// - Create an UISystem object
    /// - Add DragHandler2D to objects you want to be draggable
    /// - Attach a collider to the draggable object
    class DragHandler2D : public SomeDragHandler {
    public:
        using Base = SomeDragHandler;

        using OnDragUpdateFunc = std::function<void(DragHandler2D&)>;

        OnDragUpdateFunc onDragUpdateFunc;

        /// What to do when the object is dropped
        enum class DropType {
            Stay,    // Stay at position where it was dropped
            SnapBack // Snap back to original position
        };

        DropType dropType = DropType::Stay;

        virtual void Drop();

        DragHandler2D& SetOnDragUpdateFunc(OnDragUpdateFunc onDragUpdateFunc) {
            this->onDragUpdateFunc = onDragUpdateFunc;
            return *this;
        }

        // MARK: SomeDragHandler

        void OnDragUpdate(WorldPosition inputPosition) override;
        void OnDragEnd() override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "DragHandler2D";
        }
    };
} // namespace PJ
