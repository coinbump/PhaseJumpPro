#pragma once

#include "SomeRandom.h"
#include "SomeRandomChoice.h"
#include "VectorList.h"
#include "Weight.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/2/24
 */
namespace PJ {
    /// Each choice has a weight that affects its random probability
    /// Useful for cases where we want randomness, but prefer certain outcomes
    template <class Choice>
    class WeightedRandomChoice {
    protected:
        using Weight = SomeWeight<Choice>;
        using WeightList = VectorList<UP<Weight>>;

        WeightList choices;

    public:
        WeightedRandomChoice() {}

        void Add(UP<Weight>& weight) {
            choices.push_back(std::move(weight));
        }

        std::optional<Choice> Choose(SomeRandom& random) {
            return ChooseAt(random.Value());
        }

        std::optional<Choice> ChooseAt(float factor) {
            VectorList<PJ::Weight<Choice>> evaluateWeights;

            float totalWeight = 0;
            for (auto& wr : choices) {
                float weight = wr->Value();
                PJ::Add(evaluateWeights, PJ::Weight(weight, wr->choice));

                totalWeight += weight;
            }

            GUARDR(totalWeight > 0, {})

            float random = totalWeight * factor;

            float curWeight = 0;
            for (auto& wr : evaluateWeights) {
                float weight = wr.Value();
                GUARD_CONTINUE(weight > 0)

                curWeight += weight;
                if (random <= curWeight) {
                    return wr.choice;
                }
            }

            return {};
        }
    };
} // namespace PJ
