#include "RectPositioner2D.h"

using namespace std;
using namespace PJ;

PositionFunc RectPositioner2D::MakeFunc(Vector2 worldSize) {
    return [=](SomeRandom& random) {
        float halfWidth = worldSize.x / 2.0f;
        float halfHeight = worldSize.y / 2.0f;

        float distanceX = random.Delta(halfWidth);
        float distanceY = random.Delta(halfHeight);

        Vector3 result(distanceX, distanceY, 0);

        return result;
    };
}
