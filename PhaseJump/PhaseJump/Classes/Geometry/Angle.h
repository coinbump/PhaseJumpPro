#pragma once

#include "FloatMath.h"
#include "Macros_Operators.h"
#include "MathUtils.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/13/24
 */
namespace PJ {
    struct Angle {
    protected:
        float value = 0;

        Angle(float value) :
            value(value) {}

    public:
        using This = Angle;

        struct Config {
            float degrees{};
        };

        Angle() :
            value(0) {}

        Angle(Config config) :
            value(config.degrees) {}

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

        Angle(Vector2 distance);

        Angle Clipped() const;

        void Clip() {
            SetDegrees(Clipped().Degrees());
        }

        static Angle WithRadians(float radians) {
            return Angle(radians * FloatMath::RadiansToDegrees);
        }

        static Angle WithDegrees(float degrees) {
            return Angle(degrees);
        }

        Vector2 ToVector2(float magnitude = 1.0f) const;

        /// @return Returns the closest turn between two angles.
        /// EXAMPLE: closest turn between angle 3 and angle 359 is -4, not 356.
        Angle MinAngleTo(Angle angle) const;

        MATH_OPERATORS(Angle, value)

        operator Vector2() const {
            return ToVector2();
        }

        Angle operator-() const {
            Angle result = *this;
            result.value = -result.value;
            return result;
        }

        static Angle const zero;
    };
} // namespace PJ
