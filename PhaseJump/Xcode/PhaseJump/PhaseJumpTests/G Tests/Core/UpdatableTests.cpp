#include "gtest/gtest.h"
#include "Updatable.h"

using namespace PJ;
using namespace std;

namespace UpdatableTests {
}

using namespace UpdatableTests;

TEST(Updatable, TestIsFinished)
{
    bool didFinish{};
    
    Updatable sut;
    sut.onFinishFunc = [&](auto& updatable) {
        didFinish = true;
    };
    sut.onUpdateFunc = [&](auto& updatable, auto time) {
        return FinishType::Finish;
    };
    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate({1});
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_TRUE(didFinish);
}

TEST(Updatable, Finish)
{
    bool didFinish{};
    
    Updatable sut;
    sut.onFinishFunc = [&](auto& updatable) {
        didFinish = true;
    };
    EXPECT_FALSE(sut.IsFinished());
    sut.Finish();
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_TRUE(didFinish);
}

TEST(Updatable, Update)
{
    float timeValue{};
    
    Updatable sut;
    sut.onUpdateFunc = [&](auto& updatable, auto time) {
        timeValue += time.delta;
        return FinishType::Continue;
    };
    EXPECT_FALSE(sut.IsFinished());
    
    sut.OnUpdate({3});
    EXPECT_EQ(3, timeValue);
}


TEST(Updatable, UpdateAfterFinish)
{
    float timeValue{};
    
    Updatable sut;
    sut.onUpdateFunc = [&](auto& updatable, auto time) {
        timeValue += time.delta;
        return FinishType::Continue;
    };
    sut.Finish();
    EXPECT_TRUE(sut.IsFinished());
    
    sut.OnUpdate({3});
    EXPECT_EQ(0, timeValue);
}
