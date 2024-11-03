#include "gtest/gtest.h"
#include "RateLimiter.h"
#include "Funcs.h"

using namespace PJ;
using namespace std;

namespace RateLimiterTests {
}

using namespace RateLimiterTests;

TEST(RateLimiter, OnFire)
{
    auto sut = MAKE<RateLimiter>(3);
    auto value = MAKE<int>(0);

    auto baseOnFireFunc = sut->onFireFunc;
    sut->onFireFunc = [=](auto& owner) {
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

TEST(RateLimiter, OverrideOnFire)
{
    auto sut = MAKE<RateLimiter>(3);
    auto value = MAKE<int>(0);

    SomeLimiter::OnFireFunc overrideFunc = [=](auto& owner) {
        *value += 1;
    };

    sut->onFireFunc = OverrideFunc(sut->onFireFunc, overrideFunc);

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
