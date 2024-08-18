#pragma once

#include "Polygon.h"
#include "SomeCollider2D.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/12/24
 */
namespace PJ {
    /// 2D polygon collider
    class PolygonCollider2D : public SomeCollider2D {
    public:
        Polygon poly;
    };

    class CircleCollider2D {
    public:
        float radius = 1;
    };

    class CapsuleCollider2D {
        Vector2 size;
    };
} // namespace PJ
