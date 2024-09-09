#pragma once

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple protocol
 Code review: 7/18/24
 */
namespace PJ {
    /// When the pointer is over the object, IsHovering becomes true
    /// (set by UI system only)
    class SomeHoverGestureHandler : public WorldComponent<> {
    public:
        virtual void SetIsHovering(bool value) = 0;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SomeHoverGestureHandler";
        }
    };
} // namespace PJ
