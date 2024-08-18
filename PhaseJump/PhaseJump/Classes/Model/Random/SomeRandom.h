#pragma once

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    /// Returns a normalized random value between 0-1.0
    /// This exists so we can inject specific random numbers for unit tests
    class SomeRandom {
    public:
        virtual ~SomeRandom() {}

        /// Returns value between 0-1.0
        virtual float Value() = 0;

        /// Vary value between +/- vary random max
        float VaryFloat(float value, float vary) {
            if (0 == vary) {
                return value;
            }
            return value + Delta(vary);
        }

        /// Returns a random value between -delta...delta.
        float Delta(float delta) {
            return Value() * delta * 2.0f - delta;
        }
    };
} // namespace PJ
