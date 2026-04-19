#include "RectCollider2D.h"
#include <cmath>

using namespace std;
using namespace PJ;

RectCollider2D& RectCollider2D::SetSize(Vector2 value) {
    GUARDR(value.x >= 0 && value.y >= 0, *this);
    size = value;
    return *this;
}

bool RectCollider2D::TestHit(Vector2 position) {
    float halfX = size.x / 2.0f;
    float halfY = size.y / 2.0f;
    return std::abs(position.x) <= halfX && std::abs(position.y) <= halfY;
}

Vector3 RectCollider2D::WorldSize() const {
    return { size.x, size.y, 0 };
}

void RectCollider2D::SetWorldSize(Vector3 value) {
    SetSize({ value.x, value.y });
}
