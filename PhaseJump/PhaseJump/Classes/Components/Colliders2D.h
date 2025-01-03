#pragma once

#include "Polygon.h"
#include "SomeCollider2D.h"

/*
 RATING: 5 stars
 Tested and works
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

        // MARK: SomeCollider2D

        bool TestHit(Vector2 position) override {
            return poly.TestHit(position);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "PolygonCollider2D";
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override;
        void SetWorldSize(Vector3 value) override;
    };

    // 2D circle collider
    class CircleCollider2D : public SomeCollider2D {
    public:
        float radius = 1;

        CircleCollider2D(float radius) :
            radius(radius) {}

        // MARK: SomeCollider2D

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
    //    class CapsuleCollider2D : public SomeCollider2D {
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
