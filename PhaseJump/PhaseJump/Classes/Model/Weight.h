#pragma once

#include "Transformer.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/3/24
 PORTED TO: C++, C#
 */
namespace PJ {
    /// Specifies the weight of a random event
    template <class Core>
    class Weight {
    protected:
        /// Base weight (never changes)
        float weight = 1.0f;

    public:
        using Transform = PJ::SomeTransformer<Weight<Core>>;

        Core core{};

        /// Optional object that dynamically adjusts the weight during
        /// evaluation
        SP<Transform> adjust;

        Weight(float weight) :
            weight(weight) {}

        Weight(float weight, Core core) :
            weight(weight),
            core(core) {}

        float Value() const {
            return weight;
        }

        void SetValue(float value) {
            weight = value;
        }
    };
} // namespace PJ
