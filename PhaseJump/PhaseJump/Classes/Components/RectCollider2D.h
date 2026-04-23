#pragma once

#include "Collider2D.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/18/26
 */
namespace PJ {
    /// 2D axis-aligned rectangle collider (centered at origin)
    class RectCollider2D : public Collider2D {
    protected:
        Vector2 size{ 1, 1 };

    public:
        using This = RectCollider2D;

        RectCollider2D(Vector2 size = { 1, 1 }) {
            SetSize(size);
        }

        Vector2 Size() const {
            return size;
        }

        This& SetSize(Vector2 value);

        // MARK: Collider2D

        bool TestHit(Vector2 position) override;
        Bounds2D GetBounds() override;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "RectCollider2D";
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override;
        void SetWorldSize(Vector3 value) override;
    };
} // namespace PJ
