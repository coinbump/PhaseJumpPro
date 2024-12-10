#include "Colliders2D.h"

using namespace std;
using namespace PJ;

Vector3 PolygonCollider2D::WorldSize() const {
    return poly.Size();
}

void PolygonCollider2D::SetWorldSize(Vector3 value) {
    poly.SetSize(value);
}

// MARK: - CircleCollider2D

Vector3 CircleCollider2D::WorldSize() const {
    return { radius * 2.0f, radius * 2.0f, 0 };
}

void CircleCollider2D::SetWorldSize(Vector3 value) {
    float maxValue = std::max(value.x, value.y);
    radius = maxValue / 2.0f;
}
