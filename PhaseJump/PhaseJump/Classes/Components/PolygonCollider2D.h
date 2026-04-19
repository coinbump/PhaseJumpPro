#pragma once

#include "Collider2D.h"
#include "Polygon.h"

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 4/14/26
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
} // namespace PJ
