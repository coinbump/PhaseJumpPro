#include "StandardRandom.h"
#include <chrono>
#include <mutex>

// TODO: can we use new DependencyContainer for this
// CODE REVIEW: ?/23
namespace PJ {
    static bool hasRandomSeed = false;

    /// A psuedorandom number generator. For true random numbers, see: `std::random_device`
    static std::default_random_engine generator;
    static std::mutex generateSeedMutex;

    float StandardRandom::Value() {
        if (!hasRandomSeed) {
            std::lock_guard guard(generateSeedMutex);

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
} // namespace PJ
