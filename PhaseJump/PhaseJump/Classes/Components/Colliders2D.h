#pragma once

#include "Collider2D.h"
#include "Polygon.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/12/24
 */
namespace PJ {
    /// 2D polygon collider
    class PolygonCollider2D : public Collider2D {
    protected:
        /// Stores the collider shape
        Polygon poly;

        /// Stores the collider size. We don't resize the polygon itself because if the polygon was
        /// ever resized to zero, it would become locked at size zero (can't scale a zero size
        /// object)
        Vector2 size;

    public:
        using This = PolygonCollider2D;

        This& SetPolygon(Polygon const& value);

        // MARK: Collider2D

        bool TestHit(Vector2 position) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "PolygonCollider2D";
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override;
        void SetWorldSize(Vector3 value) override;
    };

    // 2D circle collider
    class CircleCollider2D : public Collider2D {
    public:
        float radius = 1;

        CircleCollider2D(float radius) :
            radius(radius) {}

        // MARK: Collider2D

        bool TestHit(Vector2 position) override {
            auto distance = sqrt(position.x * position.x + position.y * position.y);
            return distance <= radius;
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "CircleCollider2D";
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override;
        void SetWorldSize(Vector3 value) override;
    };

    // FUTURE:
    //    class CapsuleCollider2D : public Collider2D {
    //    public:
    //        Vector2 size;
    //
    //        // MARK: SomeWorldComponent
    //
    //        String TypeName() const override {
    //            return "CapsuleCollider2D";
    //        }
    //    };
} // namespace PJ
