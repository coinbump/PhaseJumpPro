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
