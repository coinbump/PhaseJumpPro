#include "gtest/gtest.h"
#include "PatternRateLimiter.h"
#include "Funcs.h"

using namespace PJ;
using namespace std;

namespace PatternRateLimiterTests {
}

using namespace PatternRateLimiterTests;

TEST(PatternRateLimiter, OnFire)
{
    VectorList<PatternRateLimiter::Pattern> patterns{
        PatternRateLimiter::DelayPattern{1},
        PatternRateLimiter::RateCountPattern{1, 2},
        PatternRateLimiter::DelayPattern{2},
        PatternRateLimiter::RateCountPattern{3, 3}
    };

    PatternRateLimiter sut(patterns);

    int fireValue = 0;
    sut.onFireFunc = [&](auto& limiter) {
        fireValue++;
    };

    sut.OnUpdate({1});
    EXPECT_EQ(1, sut.PatternIndex());

    sut.Fire();
    EXPECT_EQ(1, fireValue);
    EXPECT_EQ(0, sut.SequenceCount());

    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(2, fireValue);
    EXPECT_EQ(2, sut.PatternIndex());
    EXPECT_EQ(1, sut.SequenceCount());

    // Delay- 50%
    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(2, fireValue);
    EXPECT_EQ(2, sut.PatternIndex());
    EXPECT_EQ(1, sut.SequenceCount());

    // Delay- Finish
    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(3, fireValue);
    EXPECT_EQ(3, sut.PatternIndex());
    EXPECT_EQ(1, sut.SequenceCount());

    sut.OnUpdate({3});
    sut.Fire();
    EXPECT_EQ(4, fireValue);
    EXPECT_EQ(3, sut.PatternIndex());
    EXPECT_EQ(1, sut.SequenceCount());

    sut.OnUpdate({3});
    sut.Fire();
    EXPECT_EQ(5, fireValue);
    EXPECT_EQ(0, sut.PatternIndex());
    EXPECT_EQ(2, sut.SequenceCount());

    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(6, fireValue);
    EXPECT_EQ(1, sut.PatternIndex());
    EXPECT_EQ(2, sut.SequenceCount());
}

TEST(PatternRateLimiter, BurstFire)
{
    VectorList<PatternRateLimiter::Pattern> patterns{
        PatternRateLimiter::RateCountPattern{1, 3},
        PatternRateLimiter::DelayPattern{2},
    };

    PatternRateLimiter sut(patterns);

    int fireValue = 0;
    sut.onFireFunc = [&](auto& limiter) {
        fireValue++;
    };

    sut.Fire();
    EXPECT_EQ(1, fireValue);
    EXPECT_EQ(0, sut.PatternIndex());
    EXPECT_EQ(0, sut.SequenceCount());

    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(2, fireValue);
    EXPECT_EQ(0, sut.PatternIndex());
    EXPECT_EQ(0, sut.SequenceCount());

    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(3, fireValue);
    EXPECT_EQ(1, sut.PatternIndex());
    EXPECT_EQ(1, sut.SequenceCount());

    // Delay- 50%
    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(3, fireValue);
    EXPECT_EQ(1, sut.PatternIndex());
    EXPECT_EQ(1, sut.SequenceCount());

    // Delay- Finish
    sut.OnUpdate({1});
    sut.Fire();
    EXPECT_EQ(4, fireValue);
    EXPECT_EQ(0, sut.PatternIndex());
    EXPECT_EQ(1, sut.SequenceCount());
}
