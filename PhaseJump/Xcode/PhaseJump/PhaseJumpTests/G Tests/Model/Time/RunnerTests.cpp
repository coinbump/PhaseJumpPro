#include "gtest/gtest.h"
#include "Runner.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace RunnerTests {
    class TestRunner : public Runner
    {
    public:
        using Base = Runner;

        int finishCount = 0;
        int resetCount = 0;

        TestRunner(Runner::RunType runType) : Base(runType)
        {
            onFinishFunc = [](auto& runner) { static_cast<TestRunner*>(&runner)->finishCount++; };
            onResetFunc = [](auto& runner) { static_cast<TestRunner*>(&runner)->resetCount++; };
        }
    };
}

using namespace RunnerTests;

TEST(Runner, RunOnce) {
    auto sut = TestRunner(Runner::RunType::Once);
    EXPECT_FALSE(sut.IsFinished());
    sut.SetIsFinished(false);
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(0, sut.finishCount);
    sut.SetIsFinished(true);
    EXPECT_EQ(1, sut.finishCount);
    EXPECT_EQ(0, sut.resetCount);

    sut.SetIsFinished(true);
    EXPECT_EQ(1, sut.finishCount);
    EXPECT_EQ(0, sut.resetCount);
}

TEST(Runner, Repeat) {
    auto sut = TestRunner(Runner::RunType::Repeat);
    EXPECT_FALSE(sut.IsFinished());
    sut.SetIsFinished(false);
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(0, sut.finishCount);
    sut.SetIsFinished(true);
    EXPECT_EQ(1, sut.finishCount);
    EXPECT_EQ(1, sut.resetCount);
    EXPECT_FALSE(sut.IsFinished());

    sut.SetIsFinished(true);
    EXPECT_EQ(2, sut.finishCount);
    EXPECT_EQ(2, sut.resetCount);
    EXPECT_FALSE(sut.IsFinished());
}
