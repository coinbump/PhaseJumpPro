#include "Vector2.h"
#include <stdio.h>
#include <TSVector2D.h>

using namespace PJ;

Vector2 const Vector2::down(0, -1);
Vector2 const Vector2::left(-1, 0);
Vector2 const Vector2::one(1, 1);
Vector2 const Vector2::right(1, 0);
Vector2 const Vector2::up(0, 1);
Vector2 const Vector2::zero(0, 0);

Vector2Int const Vector2Int::one(1, 1);
Vector2Int const Vector2Int::three(3, 3);

Vector2::operator Terathon::Vector2D() const { return Terathon::Vector2D(x, y); }
Vector2::operator Terathon::Point2D() const { return Terathon::Point2D(x, y); }
