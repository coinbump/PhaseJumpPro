#pragma once

#include "SomeRaycaster.h"

// : /23
namespace PJ {
    struct RaycastHit2D {
        WorldNode& node;

        RaycastHit2D(WorldNode& node) :
            node(node) {}
    };

    /// Handles 2D Raycast tests
    class SomeRaycaster2D : public SomeRaycaster {
    public:
        virtual VectorList<RaycastHit2D> Raycast(Vector2 origin, Vector2 direction) = 0;
    };
} // namespace PJ
