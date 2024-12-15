#include "Angle.h"

using namespace PJ;

const Angle Angle::zero = Angle::WithDegrees(0);

Angle::Angle(Vector2 distance) {
    // Prevent infinite/invalid angle
    if (distance.x == 0 && distance.y == 0) {
        this->value = 0;
        return;
    }

    float radians = atan2(distance.y * vecDown, distance.x);
    auto angle = Angle::WithDegrees((FloatMath::RadiansToDegrees * radians) + 90.0f);
    auto result = angle.Clipped();
    this->value = result.Degrees();
}

Angle Angle::Clipped() const {
    auto angle = Degrees();
    angle = fmod(angle, 360.0f);
    if (angle < 0) {
        angle = 360.0f - fmod(abs(angle), 360.0f);
    }
    return Angle::WithDegrees(angle);
}

Vector2 Angle::ToVector2(float magnitude) const {
    Vector2 result(0, 0);

    float radians = Radians();

    // Sin/cos can produce minute values that are difficult to unit test, so reduce their precision
    float sinVal = std::round(sin(radians) * 1000.0f) / 1000.0f;
    float cosVal = std::round(cos(radians) * 1000.0f) / 1000.0f;

    float x = magnitude * sinVal;
    float y = magnitude * cosVal * vecUp;
    result.x = x;
    result.y = y;

    return result;
}

Angle Angle::MinAngleTo(Angle angle) const {
    float firstAngle = Degrees();
    float finalAngle = angle.Degrees();

    float delta1, delta2;
    float result = 0;

    if (finalAngle > firstAngle) {
        delta1 = finalAngle - firstAngle;
        delta2 = -(360.0f - delta1);
    } else {
        delta1 = -(firstAngle - finalAngle);
        delta2 = 360.0f + delta1;
    }

    if (abs(delta1) < abs(delta2)) {
        result = delta1;
    } else {
        result = delta2;
    }

    return Angle::WithDegrees(result);
}
