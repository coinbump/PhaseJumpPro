#pragma once

#include "Raycaster.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/16/24
 */
namespace PJ {
    struct RaycastHit2D {
        WorldNode& node;

        RaycastHit2D(WorldNode& node) :
            node(node) {}
    };

    /// Handles 2D Raycast tests
    class Raycaster2D : public Raycaster {
    public:
        using RaycastHit2DList = VectorList<RaycastHit2D>;

        using RaycastFunc =
            std::function<RaycastHit2DList(Vector2 worldPosition, Vector2 direction)>;

        RaycastFunc raycastFunc;

        virtual RaycastHit2DList Raycast(Vector2 worldPos, Vector2 direction) {
            GUARDR(raycastFunc, {})
            return raycastFunc(worldPos, direction);
        }
    };
} // namespace PJ
