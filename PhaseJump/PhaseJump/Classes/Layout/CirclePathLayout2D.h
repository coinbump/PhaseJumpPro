#ifndef CIRCLEPATHLAYOUT2D_H
#define CIRCLEPATHLAYOUT2D_H

#include "SomePathLayout.h"
#include "CirclePath.h"
#include <memory>

namespace PJ
{
    class WorldSizeAble2D {
        virtual Vector2 WorldSize() const = 0;
        virtual void SetWorldSize(Vector2 value) = 0;
    };

    /// <summary>
    /// Distribute objects along a circle path
    /// </summary>
    class CirclePathLayout2D : public SomePathLayout, public WorldSizeAble2D
    {
    public:
        float radius = 1.0f;

        CirclePathLayout2D() {
        }

        CirclePathLayout2D(float radius) : radius(radius) {
        }

        Vector2 WorldSize() const override
        {
            return Vector2(radius * 2.0f, radius * 2.0f);
        }

        void SetWorldSize(Vector2 value) override {
            radius = std::min(value.x, value.y) / 2.0f;
        }

        std::shared_ptr<SomePath> BuildPath() override
        {
            return std::make_shared<CirclePath>(radius);
        }
    };
}

#endif
