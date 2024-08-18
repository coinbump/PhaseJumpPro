#ifndef PJSTDNORMALRANDOM_H
#define PJSTDNORMALRANDOM_H

#include "SomeRandom.h"
#include <random>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: evaluate different random number generators and seeds
    class StandardRandom : public SomeRandom {
    public:
        float Value() override;
    };
} // namespace PJ

#endif
