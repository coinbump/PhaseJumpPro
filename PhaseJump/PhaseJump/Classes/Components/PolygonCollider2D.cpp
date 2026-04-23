#include "PolygonCollider2D.h"

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

Bounds2D PolygonCollider2D::GetBounds() {
    auto poly = this->poly;
    poly.SetSize(size);

    Vector2 min = poly.Min();
    Vector2 max = poly.Max();
    Vector2 extents = (max - min) / 2.0f;
    Vector2 center = (max + min) / 2.0f;
    return Bounds2D(center, extents);
}
