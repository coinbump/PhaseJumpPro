#include "StandardRandom.h"
#include <chrono>
#include <mutex>

using namespace PJ;

static bool hasRandomSeed = false;

/// A psuedorandom number generator. For true random numbers, see: `std::random_device`
static std::default_random_engine generator;
static std::mutex generateSeedMutex;

void GenerateSeedIfNeeded() {
    GUARD(hasRandomSeed)

    std::lock_guard guard(generateSeedMutex);

    hasRandomSeed = true;

    using Clock = std::chrono::high_resolution_clock;
    Clock::duration d = Clock::now().time_since_epoch();
    uint32_t seed = (uint32_t)d.count();

    generator.seed(seed);
}

float StandardRandom::Value() {
    GenerateSeedIfNeeded();

    std::uniform_real_distribution<float> distribution(0, 1.0f);
    return distribution(generator);
}

int StandardRandom::IntValue(int start, int end) {
    GenerateSeedIfNeeded();

    std::uniform_int_distribution<> distribution(start, end);
    return distribution(generator);
}
