#include "Vector2.h"
#include <stdio.h>
#include <TSVector2D.h>

using namespace PJ;

Vector2 const Vector2::left{ vecLeft, 0 };
Vector2 const Vector2::right{ vecRight, 0 };
Vector2 const Vector2::up{ 0, vecUp };
Vector2 const Vector2::down{ 0, vecDown };
Vector2 const Vector2::upLeft{ vecLeft, vecUp };
Vector2 const Vector2::downRight{ vecRight, vecDown };
Vector2 const Vector2::one{ 1, 1 };
Vector2 const Vector2::max{ FloatMath::maxValue, FloatMath::maxValue };

Vector2Int const Vector2Int::zero(0, 0);
Vector2Int const Vector2Int::one(1, 1);
Vector2Int const Vector2Int::three(3, 3);

Vector2::operator Terathon::Vector2D() const {
    return Terathon::Vector2D(x, y);
}

Vector2::operator Terathon::Point2D() const {
    return Terathon::Point2D(x, y);
}

std::ostream& PJ::operator<<(std::ostream& os, Vector2 const& value) {
    os << std::format("{{{}, {}}}", value.x, value.y);
    return os;
}

String Vector2::ToString() const {
    std::stringstream stream;
    stream << *this;
    return stream.str();
}

// MARK: Vector2Int

std::ostream& PJ::operator<<(std::ostream& os, Vector2Int const& value) {
    os << std::format("{{{}, {}}}", value.x, value.y);
    return os;
}

String Vector2Int::ToString() const {
    std::stringstream stream;
    stream << *this;
    return stream.str();
}
