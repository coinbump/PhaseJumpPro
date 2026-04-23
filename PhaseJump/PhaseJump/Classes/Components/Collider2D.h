#pragma once

#include "Bounds2D.h"
#include "Collider.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/23/26
 */
namespace PJ {
    /// 2D collider
    class Collider2D : public WorldComponent, public WorldSizeable {
    public:
        using TestHitFunc = std::function<bool(Vector2 position)>;

        TestHitFunc testHitFunc;

        virtual bool TestHit(Vector2 position) {
            GUARDR(testHitFunc, false);
            return testHitFunc(position);
        }

        /// @return Axis-aligned 2D bounds that contain the collider's shape in its local
        /// space. Callers that need world-space bounds must translate by the owner node's
        /// position.
        virtual Bounds2D GetBounds() = 0;

        /// @return True if bounds overlaps this collider's bounds. A cheap broad-phase
        /// check suitable for culling. Use TestHit for precision
        bool TestIntersect(Bounds2D bounds) {
            return GetBounds().TestIntersect(bounds);
        }
    };
} // namespace PJ
