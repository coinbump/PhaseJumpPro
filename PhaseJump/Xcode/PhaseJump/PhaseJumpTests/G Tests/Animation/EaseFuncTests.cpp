#include "gtest/gtest.h"

#include "EaseFunc.h"

#include <cstring>

using namespace std;
using namespace PJ;

namespace EaseFuncTests {
    constexpr float tolerance = 0.0005f;

    // Easings that should pass through (0, 0) and (1, 1) exactly and stay within [0, 1].
    struct BoundedEase {
        const char* name;
        EaseFunc func;
    };

    BoundedEase const kBoundedEases[] = {
        { "linear", EaseFuncs::linear },
        { "inSine", EaseFuncs::inSine },
        { "outSine", EaseFuncs::outSine },
        { "inOutSine", EaseFuncs::inOutSine },
        { "inSquared", EaseFuncs::inSquared },
        { "outSquared", EaseFuncs::outSquared },
        { "inOutSquared", EaseFuncs::inOutSquared },
        { "inCubed", EaseFuncs::inCubed },
        { "outCubed", EaseFuncs::outCubed },
        { "inOutCubed", EaseFuncs::inOutCubed },
        { "inQuart", EaseFuncs::inQuart },
        { "outQuart", EaseFuncs::outQuart },
        { "inOutQuart", EaseFuncs::inOutQuart },
        { "inQuint", EaseFuncs::inQuint },
        { "outQuint", EaseFuncs::outQuint },
        { "inOutQuint", EaseFuncs::inOutQuint },
        { "inExpo", EaseFuncs::inExpo },
        { "outExpo", EaseFuncs::outExpo },
        { "inOutExpo", EaseFuncs::inOutExpo },
        { "inCirc", EaseFuncs::inCirc },
        { "outCirc", EaseFuncs::outCirc },
        { "inOutCirc", EaseFuncs::inOutCirc },
        { "inBounce", EaseFuncs::inBounce },
        { "outBounce", EaseFuncs::outBounce },
        { "inOutBounce", EaseFuncs::inOutBounce },
    };

    // Back / Elastic overshoot mid-range but still anchor at (0, 0) and (1, 1).
    EaseFunc const kOvershootingEases[] = {
        EaseFuncs::inBack,        EaseFuncs::outBack,    EaseFuncs::inOutBack,
        EaseFuncs::inElastic,     EaseFuncs::outElastic, EaseFuncs::inOutElastic,
    };
} // namespace

using namespace EaseFuncTests;

TEST(EaseFuncs, Linear_IsIdentity) {
    EXPECT_FLOAT_EQ(0.0f, EaseFuncs::linear(0.0f));
    EXPECT_FLOAT_EQ(0.25f, EaseFuncs::linear(0.25f));
    EXPECT_FLOAT_EQ(0.5f, EaseFuncs::linear(0.5f));
    EXPECT_FLOAT_EQ(0.75f, EaseFuncs::linear(0.75f));
    EXPECT_FLOAT_EQ(1.0f, EaseFuncs::linear(1.0f));
}

TEST(EaseFuncs, BoundedEases_AnchorAtZeroAndOne) {
    for (auto const& ease : kBoundedEases) {
        SCOPED_TRACE(ease.name);
        EXPECT_NEAR(0.0f, ease.func(0.0f), tolerance);
        EXPECT_NEAR(1.0f, ease.func(1.0f), tolerance);
    }
}

TEST(EaseFuncs, BoundedEases_StayInRange) {
    for (auto const& ease : kBoundedEases) {
        SCOPED_TRACE(ease.name);
        for (int i = 0; i <= 20; i++) {
            float t = (float)i / 20.0f;
            float v = ease.func(t);
            EXPECT_GE(v, -tolerance);
            EXPECT_LE(v, 1.0f + tolerance);
        }
    }
}

TEST(EaseFuncs, OvershootingEases_AnchorAtZeroAndOne) {
    // Back / Elastic overshoot mid-range, but endpoints must be exact.
    for (auto const& func : kOvershootingEases) {
        EXPECT_NEAR(0.0f, func(0.0f), tolerance);
        EXPECT_NEAR(1.0f, func(1.0f), tolerance);
    }
}

TEST(EaseFuncs, PowerFamilies_KnownMidpoints) {
    // All "in" power curves should equal (0.5)^n at the midpoint.
    EXPECT_FLOAT_EQ(0.25f, EaseFuncs::inSquared(0.5f));       // 0.5^2
    EXPECT_FLOAT_EQ(0.125f, EaseFuncs::inCubed(0.5f));        // 0.5^3
    EXPECT_FLOAT_EQ(0.0625f, EaseFuncs::inQuart(0.5f));       // 0.5^4
    EXPECT_FLOAT_EQ(0.03125f, EaseFuncs::inQuint(0.5f));      // 0.5^5

    // "out" curves are the mirror: 1 - (1 - 0.5)^n = 1 - 0.5^n
    EXPECT_FLOAT_EQ(0.75f, EaseFuncs::outSquared(0.5f));
    EXPECT_FLOAT_EQ(0.875f, EaseFuncs::outCubed(0.5f));
    EXPECT_FLOAT_EQ(0.9375f, EaseFuncs::outQuart(0.5f));
    EXPECT_FLOAT_EQ(0.96875f, EaseFuncs::outQuint(0.5f));

    // "inOut" curves pass through 0.5 at 0.5.
    EXPECT_NEAR(0.5f, EaseFuncs::inOutSquared(0.5f), tolerance);
    EXPECT_NEAR(0.5f, EaseFuncs::inOutCubed(0.5f), tolerance);
    EXPECT_NEAR(0.5f, EaseFuncs::inOutQuart(0.5f), tolerance);
    EXPECT_NEAR(0.5f, EaseFuncs::inOutQuint(0.5f), tolerance);
}

TEST(EaseFuncs, Sine_KnownValues) {
    // inSine(1) = 1 - cos(π/2) = 1
    // outSine(0) = sin(0) = 0
    // inOutSine(0.5) = -(cos(π/2) - 1)/2 = 0.5
    EXPECT_NEAR(1.0f - cosf(0.25f * 3.14159265f), EaseFuncs::inSine(0.5f), tolerance);
    EXPECT_NEAR(sinf(0.25f * 3.14159265f), EaseFuncs::outSine(0.5f), tolerance);
    EXPECT_NEAR(0.5f, EaseFuncs::inOutSine(0.5f), tolerance);
}

TEST(EaseFuncs, Circ_Midpoint) {
    // inCirc(0.5) = 1 - sqrt(1 - 0.25) = 1 - sqrt(0.75)
    EXPECT_NEAR(1.0f - sqrtf(0.75f), EaseFuncs::inCirc(0.5f), tolerance);
    // outCirc(0.5) = sqrt(1 - 0.25) = sqrt(0.75)
    EXPECT_NEAR(sqrtf(0.75f), EaseFuncs::outCirc(0.5f), tolerance);
    // inOutCirc passes through 0.5 at 0.5
    EXPECT_NEAR(0.5f, EaseFuncs::inOutCirc(0.5f), tolerance);
}

TEST(EaseFuncs, Bounce_IsSymmetric) {
    for (int i = 0; i <= 10; i++) {
        float t = (float)i / 10.0f;
        EXPECT_NEAR(EaseFuncs::inBounce(t), 1.0f - EaseFuncs::outBounce(1.0f - t), tolerance);
    }
}

TEST(EaseFuncs, NonBounceEases_MonotonicallyNonDecreasing) {
    // Non-overshooting, non-bouncing eases should never go backwards across a smooth sweep.
    // (Bounce easings intentionally oscillate within [0, 1].)
    for (auto const& ease : kBoundedEases) {
        if (std::strstr(ease.name, "Bounce") != nullptr) {
            continue;
        }
        SCOPED_TRACE(ease.name);
        float prev = ease.func(0.0f);
        for (int i = 1; i <= 100; i++) {
            float t = (float)i / 100.0f;
            float v = ease.func(t);
            EXPECT_GE(v, prev - tolerance);
            prev = v;
        }
    }
}
