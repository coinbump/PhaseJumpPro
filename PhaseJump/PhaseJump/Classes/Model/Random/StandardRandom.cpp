#include "StandardRandom.h"
#include <chrono>

namespace PJ
{
    static bool hasRandomSeed = false;
    static std::default_random_engine generator;

    float StandardRandom::Value()
    {
        if (!hasRandomSeed) {
            hasRandomSeed = true;

            typedef std::chrono::high_resolution_clock myclock;
            myclock::time_point beginning = myclock::now();
            myclock::duration d = myclock::now() - beginning;
            unsigned seed = (unsigned)d.count();

            generator.seed(seed);
        }

        std::uniform_real_distribution<float> distribution(0, 1.0f);
        return distribution(generator);
    }
}
