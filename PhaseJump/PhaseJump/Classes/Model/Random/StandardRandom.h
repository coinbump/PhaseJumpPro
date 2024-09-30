#pragma once

#include "SomeRandom.h"
#include <random>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/19/24
 */
namespace PJ {
    /// The default random number generator
    /// This references a global static generator, so it's ok to make copies
    class StandardRandom : public SomeRandom {
    public:
        // FUTURE: evaluate different random number generators and seeds

        // MARK: SomeRandom

        float Value() override;
        int IntValue(int start, int end) override;
    };
} // namespace PJ
