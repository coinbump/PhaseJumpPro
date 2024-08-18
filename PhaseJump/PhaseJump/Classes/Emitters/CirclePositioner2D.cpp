#include "CirclePositioner2D.h"

using namespace std;
using namespace PJ;

PositionFunc CirclePositioner2D::MakeFunc(float radius) {
    return [=](SomeRandom& random) {
        float distance = random.VaryFloat(0, 1.0f) * radius;
        float angle = random.Value() * 360.0f;

        Vector2 offset = (Vector2)Angle::DegreesAngle(angle) * distance;
        Vector3 result(offset.x, offset.y, 0);

        return result;
    };
}
