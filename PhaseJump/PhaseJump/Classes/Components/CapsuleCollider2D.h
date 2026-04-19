#pragma once

#include "Collider2D.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/18/26
 */
namespace PJ {
    /// 2D capsule collider (vertical axis). `height` is the total height (including caps).
    class CapsuleCollider2D : public Collider2D {
    protected:
        float radius = 1;
        float height = 2;

    public:
        using This = CapsuleCollider2D;

        CapsuleCollider2D(float height, float radius) {
            SetHeight(height);
            SetRadius(radius);
        }

        float Radius() const {
            return radius;
        }

        This& SetRadius(float value);

        float Height() const {
            return height;
        }

        This& SetHeight(float value);

        // MARK: Collider2D

        bool TestHit(Vector2 position) override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "CapsuleCollider2D";
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override;
        void SetWorldSize(Vector3 value) override;
    };
} // namespace PJ
