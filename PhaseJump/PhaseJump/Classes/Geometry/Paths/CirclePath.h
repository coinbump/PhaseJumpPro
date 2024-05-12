#ifndef PJCIRCLEPATH_H
#define PJCIRCLEPATH_H

#include "Angle.h"
#include "SomePath.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/7/22
 */
namespace PJ
{
    /// <summary>
    /// Model that defines a circle path
    /// </summary>
    class CirclePath : public SomePath
    {
    public:
        float radius;

        CirclePath(float radius) : radius(radius)
        {
        }

        struct PositionAtArgs {
            float progress = 0.0;
        };

        void Foo(PositionAtArgs) {
        }

        void Food2() {
            Foo({.progress = 1.0f});
        }

        Vector3 PositionAt(float progress) const override
        {
            auto result = Angle::DegreesAngle(progress * 360.0f).ToVector2(radius);
            return Vector3(result.x, result.y, 0);
        }
    };
}

#endif
