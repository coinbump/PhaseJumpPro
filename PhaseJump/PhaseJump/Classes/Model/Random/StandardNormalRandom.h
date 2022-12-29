#ifndef PJSTDNORMALRANDOM_H
#define PJSTDNORMALRANDOM_H

#include "SomeNormalRandom.h"
#include <random>

namespace PJ
{
    // TODO: evaluate different random number generators and seeds
    class StandardNormalRandom : public SomeNormalRandom
    {
    public:
        float Value() override;
    };
}

#endif
