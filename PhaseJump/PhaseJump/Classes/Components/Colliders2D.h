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

        auto& SetPoly(Polygon const& value) {
            poly = value;
            return *this;
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "PolygonCollider2D";
        }
    };

    class CircleCollider2D : public SomeCollider2D {
    public:
        float radius = 1;

        CircleCollider2D(float radius) :
            radius(radius) {}

        bool TestHit(Vector2 position) {
            auto distance = sqrt(position.x * position.x + position.y * position.y);
            return distance <= radius;
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "CircleCollider2D";
        }
    };

    class CapsuleCollider2D : public SomeCollider2D {
    public:
        Vector2 size;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "CapsuleCollider2D";
        }
    };
} // namespace PJ
