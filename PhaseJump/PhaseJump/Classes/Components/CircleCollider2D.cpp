#include "CircleCollider2D.h"
#include <algorithm>

using namespace std;
using namespace PJ;

Vector3 CircleCollider2D::WorldSize() const {
    return { radius * 2.0f, radius * 2.0f, 0 };
}

void CircleCollider2D::SetWorldSize(Vector3 value) {
    float minValue = std::min(value.x, value.y);
    SetRadius(minValue / 2.0f);
}

CircleCollider2D& CircleCollider2D::SetRadius(float value) {
    GUARDR(value >= 0, *this);
    radius = value;
    return *this;
}
