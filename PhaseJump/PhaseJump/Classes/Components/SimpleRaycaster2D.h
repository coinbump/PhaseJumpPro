#pragma once

#include "Raycaster2D.h"
#include <memory>

/*
 RATING: 4 stars
 Tested and works (test hit only, no ray casting)
 CODE REVIEW: 1/9/25
 */
namespace PJ {
    class Collider2D;
    class WorldNode;

    /// Brute force raycaster
    class SimpleRaycaster2D : public Raycaster2D {
    public:
        VectorList<RaycastHit2D> Raycast(Vector2 worldPosition, Vector2 direction) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SimpleRaycaster2D";
        }
    };
} // namespace PJ
