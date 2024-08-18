#include "gtest/gtest.h"
#include "RateLimiter.h"

using namespace PJ;
using namespace std;

namespace RateLimiterTests {
}

using namespace RateLimiterTests;

TEST(RateLimiter, Test_RateLimiter)
{
    auto sut = RateLimiter::Make();
    sut->core.minDelta = 3.0f;
    auto value = MAKE<int>(0);

    auto baseOnFireFunc = sut->onFireFunc;
    sut->onFireFunc = [=](SomeLimiter<RateLimiter::Core>& owner) {
        baseOnFireFunc(owner);
        *value += 1;
    };

    sut->Fire();
    EXPECT_EQ(1, *value);

    sut->Fire();
    sut->Fire();
    EXPECT_EQ(1, *value);

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut->core.timer);
    sut->Fire();
    EXPECT_EQ(1, *value);

    sut->OnUpdate(TimeSlice(2.0f));
    EXPECT_EQ(3.0f, sut->core.timer);
    sut->Fire();
    EXPECT_EQ(0, sut->core.timer);
    EXPECT_EQ(2, *value);
}
