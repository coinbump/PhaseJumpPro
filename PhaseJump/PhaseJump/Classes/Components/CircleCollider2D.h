#pragma once

#include "Collider2D.h"

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 4/14/26
 */
namespace PJ {
    // 2D circle collider
    class CircleCollider2D : public Collider2D {
    protected:
        float radius = 1;

    public:
        using This = CircleCollider2D;

        CircleCollider2D(float radius) {
            SetRadius(radius);
        }

        float Radius() const {
            return radius;
        }

        This& SetRadius(float value);

        // MARK: Collider2D

        bool TestHit(Vector2 position) override {
            auto distance = position.x * position.x + position.y * position.y;
            return distance <= radius * radius;
        }

        Bounds2D GetBounds() override {
            return Bounds2D({}, { radius, radius });
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "CircleCollider2D";
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override;
        void SetWorldSize(Vector3 value) override;
    };
} // namespace PJ
