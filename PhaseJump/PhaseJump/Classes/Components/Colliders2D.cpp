#include "Colliders2D.h"

using namespace std;
using namespace PJ;

Vector3 PolygonCollider2D::WorldSize() const {
    return size;
}

void PolygonCollider2D::SetWorldSize(Vector3 value) {
    size = value;
}

PolygonCollider2D& PolygonCollider2D::SetPolygon(Polygon const& value) {
    poly = value;
    size = poly.Size();
    return *this;
}

bool PolygonCollider2D::TestHit(Vector2 position) {
    auto poly = this->poly;
    poly.SetSize(size);

    return poly.TestHit(position);
}

// MARK: - CircleCollider2D

Vector3 CircleCollider2D::WorldSize() const {
    return { radius * 2.0f, radius * 2.0f, 0 };
}

void CircleCollider2D::SetWorldSize(Vector3 value) {
    float maxValue = std::max(value.x, value.y);
    radius = maxValue / 2.0f;
}
