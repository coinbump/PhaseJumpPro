#include "gtest/gtest.h"
#include "WorldPartLife.h"

using namespace PJ;
using namespace std;

namespace WorldPartLifeTests {
}

using namespace WorldPartLifeTests;

TEST(WorldPartLife, Lifecycle)
{
    int awakeCount = 0;
    int startCount = 0;
    WorldPartLife sut;

    auto awakeFunc = [&]() { awakeCount++; };
    auto startFunc = [&]() { startCount++; };

    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());

    sut.CheckedAwake(awakeFunc);
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_EQ(1, awakeCount);
    EXPECT_FALSE(sut.IsStarted());

    sut.CheckedAwake(awakeFunc);
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_EQ(1, awakeCount);
    EXPECT_FALSE(sut.IsStarted());

    sut.CheckedStart(startFunc);
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_EQ(1, awakeCount);
    EXPECT_TRUE(sut.IsStarted());
    EXPECT_EQ(1, startCount);

    sut.CheckedStart(startFunc);
    EXPECT_TRUE(sut.IsAwake());
    EXPECT_EQ(1, awakeCount);
    EXPECT_TRUE(sut.IsStarted());
    EXPECT_EQ(1, startCount);
}

TEST(WorldPartLife, EarlyStart)
{
    int startCount = 0;
    WorldPartLife sut;

    auto startFunc = [&]() { startCount++; };

    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());

    sut.CheckedStart(startFunc);
    EXPECT_FALSE(sut.IsAwake());
    EXPECT_FALSE(sut.IsStarted());
    EXPECT_EQ(0, startCount);
}
