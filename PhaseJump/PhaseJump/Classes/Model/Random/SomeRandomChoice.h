#pragma once

#include "SomeRandom.h"
#include <math.h>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 2/18/23
 */
namespace PJ {

    // TODO: replace with funcs, func creator

    /// Returns a random choice of the specified type
    template <class Type>
    class SomeRandomChoice {
    public:
        virtual Type Choose(SomeRandom& random) = 0;
    };

    template <class Type>
        requires std::integral<Type> || std::floating_point<Type>
    class MinMaxRandomChoice : public SomeRandomChoice<Type> {
    public:
        Type min;
        Type max;

        MinMaxRandomChoice(Type min, Type max) :
            min(min),
            max(max) {}

        Type Choose(SomeRandom& random) override {
            return min + ((max - min) * random.Value());
        }
    };

    template <>
    inline int MinMaxRandomChoice<int>::Choose(SomeRandom& random) {
        return min + (int)round(((float)max - (float)min) * random.Value());
    }

    using MinMaxIntRandomChoice = MinMaxRandomChoice<int>;
    using MinMaxFloatRandomChoice = MinMaxRandomChoice<float>;

    /// Returns value based on variance values
    class VaryIntRandomChoice : public SomeRandomChoice<int> {
    public:
        int value;
        int vary;

        VaryIntRandomChoice(int value, int vary) :
            value(value),
            vary(vary) {}

        int Choose(SomeRandom& random) override {
            float floatValue = random.VaryFloat((float)value, (float)vary);
            return (int)round(floatValue);
        }
    };

    /// Returns value based on variance values
    class VaryFloatRandomChoice : public SomeRandomChoice<float> {
    public:
        float value;
        float vary;

        VaryFloatRandomChoice(float value, float vary) :
            value(value),
            vary(vary) {}

        float Choose(SomeRandom& random) override {
            return random.VaryFloat(value, vary);
        }
    };
} // namespace PJ
