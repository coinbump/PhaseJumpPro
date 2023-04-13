#ifndef PJWEIGHTEDRANDOM_H
#define PJWEIGHTEDRANDOM_H

#include "VectorList.h"
#include "Weight.h"
#include "SomeRandom.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 2/18/23
 */
namespace PJ
{
    /// <summary>
    /// Each choice has a weight that affects its random probability
    /// Useful for cases where we want randomness, but prefer certain outcomes
    /// </summary>
    template <class Value>
    class WeightedRandomChoice
    {
    protected:
        VectorList<Weight<Value>> choices;

    public:
        using Weight = Weight<Value>;

        WeightedRandomChoice()
        {
        }

        void Add(Weight weight)
        {
            choices.Add(weight);
        }

        Value Choose(SomeRandom& random)
        {
            auto weight = ChooseWeight(random);
            return weight ? weight.value().value : Value();
        }

        std::optional<Weight> ChooseWeight(SomeRandom& random)
        {
            float factor = random.Value();
            auto result = ChooseWeightAt(factor);
            return result;
        }

        std::optional<Weight> ChooseWeightAt(float factor)
        {
            VectorList<Weight> adjustedWeights;

            float totalWeight = 0;
            for (auto i = choices.begin(); i != choices.end(); i++)
            {
                auto wr = *i;
                auto adjustedWeight = wr.WeightValue();
                if (nullptr != wr.adjust)
                {
                    adjustedWeight = wr.adjust->Transform(wr).WeightValue();
                }

                adjustedWeights.Add(Weight(adjustedWeight, wr.value));

                totalWeight += adjustedWeight;
            }

            // No viable choices
            if (totalWeight == 0)
            {
                return std::nullopt;
            }

            float random = totalWeight * factor;

            float curWeight = 0;
            for (auto wr : adjustedWeights)
            {
                float weight = wr.WeightValue();
                if (weight <= 0)
                {
                    continue;   // Ignore this, invalid
                }

                curWeight += weight;
                if (random <= curWeight)
                {
                    return wr;
                }
            }

            return std::nullopt;
        }
    };
}

#endif
