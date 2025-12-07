#include "gtest/gtest.h"
#include "Runner.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace RunnerTests {
}

using namespace RunnerTests;

TEST(Runner, RunOnce) {
    auto sut = Runner(RunType::Once);
    
    int finishCount = 0;
    int resetCount = 0;
    sut.onFinishFunc = [&]() { finishCount++; };
    sut.onResetFunc = [&]() { resetCount++; };

    EXPECT_FALSE(sut.IsFinished());
    sut.SetIsFinished(false);
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(0, finishCount);
    sut.SetIsFinished(true);
    EXPECT_EQ(1, finishCount);
    EXPECT_EQ(0, resetCount);

    sut.SetIsFinished(true);
    EXPECT_EQ(1, finishCount);
    EXPECT_EQ(0, resetCount);
}

TEST(Runner, Repeat) {
    auto sut = Runner(RunType::Repeat);
    
    int finishCount = 0;
    int resetCount = 0;
    sut.onFinishFunc = [&]() { finishCount++; };
    sut.onResetFunc = [&]() { resetCount++; };

    EXPECT_FALSE(sut.IsFinished());
    sut.SetIsFinished(false);
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(0, finishCount);
    sut.SetIsFinished(true);
    EXPECT_EQ(1, finishCount);
    EXPECT_EQ(1, resetCount);
    EXPECT_FALSE(sut.IsFinished());

    sut.SetIsFinished(true);
    EXPECT_EQ(2, finishCount);
    EXPECT_EQ(2, resetCount);
    EXPECT_FALSE(sut.IsFinished());
}
