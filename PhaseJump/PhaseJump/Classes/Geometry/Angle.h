#pragma once

#include "FloatMath.h"
#include "Macros_Operators.h"
#include "MathUtils.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    struct Angle {
    protected:
        float value = 0;

        Angle(float value) :
            value(value) {}

    public:
        using This = Angle;

        Angle() :
            value(0) {}

        float Degrees() const {
            return value;
        }

        void SetDegrees(float value) {
            this->value = value;
        }

        float Radians() const {
            return value * FloatMath::DegreesToRadians;
        }

        void SetRadians(float value) {
            this->value = value / FloatMath::DegreesToRadians;
        }

        Angle(Vector2 distance) {
            // Prevent infinite/invalid angle
            if (distance.x == 0 && distance.y == 0) {
                this->value = 0;
                return;
            }

            float radians = atan2(distance.y * vecDown, distance.x);
            auto angle = Angle::DegreesAngle((FloatMath::RadiansToDegrees * radians) + 90.0f);
            auto result = angle.Clipped();
            this->value = result.Degrees();
        }

        Angle Clipped() const {
            auto angle = Degrees();
            angle = fmod(angle, 360.0f);
            if (angle < 0) {
                angle = 360.0f - fmod(abs(angle), 360.0f);
            }
            return Angle::DegreesAngle(angle);
        }

        void Clip() {
            SetDegrees(Clipped().Degrees());
        }

        static Angle RadiansAngle(float radians) {
            return Angle(radians * FloatMath::RadiansToDegrees);
        }

        static Angle DegreesAngle(float degrees) {
            return Angle(degrees);
        }

        Vector2 ToVector2(float magnitude = 1.0f) const {
            Vector2 result(0, 0);

            float sinVal = sin(Radians());
            float cosVal = cos(Radians());

            float x = magnitude * sinVal;
            float y = magnitude * cosVal * vecUp;
            result.x = x;
            result.y = y;

            return result;
        }

        /// @return Returns the closest turn between two angles.
        /// EXAMPLE: closest turn between angle 3 and angle 359 is -4, not 356.
        Angle MinAngleTo(Angle angle) const {
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

            return Angle::DegreesAngle(result);
        }

        MATH_OPERATORS(Angle, value)

        operator Vector2() const {
            return ToVector2();
        }

        static Angle const zero;
    };
} // namespace PJ
