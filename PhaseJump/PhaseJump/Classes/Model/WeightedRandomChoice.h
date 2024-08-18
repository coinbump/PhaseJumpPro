#ifndef PJWEIGHTEDRANDOM_H
#define PJWEIGHTEDRANDOM_H

#include "List.h"
#include "SomeRandom.h"
#include "Weight.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/3/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Each choice has a weight that affects its random probability
    /// Useful for cases where we want randomness, but prefer certain outcomes
    template <class Value>
    class WeightedRandomChoice {
    protected:
        using WeightList = List<Weight<Value>>;
        WeightList choices;

    public:
        using Weight = Weight<Value>;

        WeightedRandomChoice() {}

        void Add(Weight weight) {
            choices.Add(weight);
        }

        Value Choose(SomeRandom& random) {
            auto weight = ChooseWeight(random);
            return weight ? weight.value().value : Value();
        }

        std::optional<Weight> ChooseWeight(SomeRandom& random) {
            float factor = random.Value();
            auto result = ChooseWeightAt(factor);
            return result;
        }

        std::optional<Weight> ChooseWeightAt(float factor) {
            WeightList adjustedWeights;

            float totalWeight = 0;
            for (auto& wr : choices) {
                auto adjustedWeight = wr.Value();
                if (wr.adjust) {
                    adjustedWeight = wr.adjust->Transform(wr).Value();
                }

                adjustedWeights.Add(Weight(adjustedWeight, wr.core));

                totalWeight += adjustedWeight;
            }

            // No viable choices
            if (totalWeight == 0) {
                return std::nullopt;
            }

            float random = totalWeight * factor;

            float curWeight = 0;
            for (auto& wr : adjustedWeights) {
                float weight = wr.Value();
                if (weight <= 0) {
                    continue; // Ignore this, invalid
                }

                curWeight += weight;
                if (random <= curWeight) {
                    return wr;
                }
            }

            return std::nullopt;
        }
    };
} // namespace PJ

#endif
