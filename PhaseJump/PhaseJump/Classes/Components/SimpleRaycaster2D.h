#pragma once

#include "SomeRaycaster2D.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class SomeCollider2D;
    class WorldNode;

    /// Brute force raycaster (slow, but simple)
    class SimpleRaycaster2D : public SomeRaycaster2D {
    public:
        VectorList<RaycastHit2D> Raycast(Vector2 worldPosition, Vector2 direction) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SimpleRaycaster2D";
        }
    };
} // namespace PJ
