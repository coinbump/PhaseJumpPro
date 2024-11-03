#pragma once

#include "MathUtils.h"
#include "SomeRandom.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/2/24
 */
namespace PJ {
    /// Generates a random choice of the specified type
    /// Alternative: compose random choice funcs instead
    template <class Choice>
    class SomeRandomChoice {
    public:
        virtual ~SomeRandomChoice() {}

        // TODO: should this return optional (see: WeightedRandomChoice)
        virtual Choice Choose(SomeRandom& random) = 0;
    };

    template <class Choice>
        requires std::integral<Choice> || std::floating_point<Choice>
    class MinMaxRandomChoice : public SomeRandomChoice<Choice> {
    public:
        Choice min;
        Choice max;

        MinMaxRandomChoice(Choice min, Choice max) :
            min(min),
            max(max) {}

        Choice Choose(SomeRandom& random) override {
            return min + ((max - min) * random.Value());
        }
    };

    template <>
    inline int MinMaxRandomChoice<int>::Choose(SomeRandom& random) {
        return min + (int)round(((float)max - (float)min) * random.Value());
    }

    using MinMaxIntRandomChoice = MinMaxRandomChoice<int>;
    using MinMaxFloatRandomChoice = MinMaxRandomChoice<float>;

    /// Generates value based on variance values
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

    /// Generates value based on variance values
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
