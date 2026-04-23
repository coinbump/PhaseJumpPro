#include "CapsuleCollider2D.h"
#include <algorithm>
#include <cmath>

using namespace std;
using namespace PJ;

CapsuleCollider2D& CapsuleCollider2D::SetRadius(float value) {
    GUARDR(value >= 0, *this);
    radius = value;
    return *this;
}

CapsuleCollider2D& CapsuleCollider2D::SetHeight(float value) {
    GUARDR(value >= 0, *this);
    height = value;
    return *this;
}

Bounds2D CapsuleCollider2D::GetBounds() {
    return Bounds2D({}, { radius, height / 2.0f });
}

bool CapsuleCollider2D::TestHit(Vector2 position) {
    float halfHeight = height / 2.0f;
    float halfRectLength = std::max(0.0f, halfHeight - radius);

    if (std::abs(position.y) <= halfRectLength) {
        return std::abs(position.x) <= radius;
    }

    float capY = (position.y > 0) ? halfRectLength : -halfRectLength;
    float dy = position.y - capY;
    float dx = position.x;
    return dx * dx + dy * dy <= radius * radius;
}

Vector3 CapsuleCollider2D::WorldSize() const {
    return { radius * 2.0f, height, 0 };
}

void CapsuleCollider2D::SetWorldSize(Vector3 value) {
    SetRadius(value.x / 2.0f);
    SetHeight(value.y);
}
