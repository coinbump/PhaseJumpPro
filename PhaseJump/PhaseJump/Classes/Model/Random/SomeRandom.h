#pragma once

#include "Macros.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 9/19/24
 */
namespace PJ {
    /// Generates random values
    class SomeRandom {
    public:
        virtual ~SomeRandom() {}

        /// @return Returns normalized value (0-1.0)
        virtual float Value() = 0;

        /// @return Returns a value in the range of `[start, end]` (inclusive)
        virtual int IntValue(int start, int end) = 0;

        /// Vary value between +/- vary random max
        float VaryFloat(float value, float vary) {
            GUARDR(vary != 0, value)
            return value + Delta(vary);
        }

        /// @return Returns a random value between -delta...delta.
        float Delta(float delta) {
            return Value() * delta * 2.0f - delta;
        }

        int Choice(int choiceCount) {
            GUARDR(choiceCount > 0, 0)
            return IntValue(0, choiceCount - 1);
        }
    };
} // namespace PJ
