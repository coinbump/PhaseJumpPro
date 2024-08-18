#ifndef CIRCLEPATHLAYOUT2D_H
#define CIRCLEPATHLAYOUT2D_H

#include "CirclePath.h"
#include "SomePathLayout.h"
#include "Utils.h"
#include "WorldSizeAble2D.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: rethink layouts using composition
    /// Distribute objects along a circle path
    class CirclePathLayout2D : public SomePathLayout, public WorldSizeAble2D {
    public:
        float radius = 1.0f;

        CirclePathLayout2D() {}

        CirclePathLayout2D(float radius) :
            radius(radius) {}

        Vector2 WorldSize2D() const override {
            return Vector2(radius * 2.0f, radius * 2.0f);
        }

        void SetWorldSize2D(Vector2 value) override {
            radius = std::min(value.x, value.y) / 2.0f;
        }

        SP<SomePath> BuildPath() override {
            return MAKE<CirclePath>(radius);
        }

        Vector3 Size() const override {
            return Vector3(radius * 2.0f, radius * 2.0f, 0);
        }
    };
} // namespace PJ

#endif
