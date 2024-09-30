#pragma once

#include "CirclePath.h"
#include "SomePathLayout.h"
#include "Utils.h"
#include "WorldSizeable.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Distribute objects along a circle path
    class CirclePathLayout2D : public SomePathLayout, public WorldSizeable {
    protected:
        float radius = 1.0f;

    public:
        CirclePathLayout2D() {}

        CirclePathLayout2D(float radius) :
            radius(radius) {}

        float Radius() const {
            return radius;
        }

        void SetRadius(float value) {
            GUARD(radius != value)
            radius = value;
            SetNeedsLayout();
        }

        // MARK: WorldSizeable

        std::optional<Vector3> WorldSize() const override {
            return Vector3(radius * 2.0f, radius * 2.0f, 0);
        }

        void SetWorldSize(Vector3 value) override {
            radius = std::min(value.x, value.y) / 2.0f;
        }

        // MARK: SomePathLayout

        SP<SomePath> BuildPath() override {
            return MAKE<CirclePath>(radius);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "CirclePathLayout2D";
        }
    };
} // namespace PJ
