#include "gtest/gtest.h"
#include "Updatables.h"
#include "WorldComponent.h"
#include "World.h"
#include "StringUtils.h"

using namespace PJ;
using namespace std;

namespace UpdatablesTests {
    class TestUpdatable : public Updatable
    {
    public:
        float finishTime = 0;
        float time = 0;

        TestUpdatable(float finishTime) : finishTime(finishTime) {
        }

        void OnUpdate(TimeSlice time) override {
            this->time += time.delta;
            isFinished = this->time >= finishTime;
        }
    };
}

using namespace UpdatablesTests;

TEST(Updatables, Toggles)
{
    auto u1 = MAKE<TestUpdatable>(1);
    auto u2 = MAKE<TestUpdatable>(2);
    Updatables sut;
    sut.Add(u1);
    sut.Add(u2);

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_FALSE(u1->IsFinished());
    EXPECT_FALSE(u2->IsFinished());
    EXPECT_EQ(0.5f, u1->time);
    EXPECT_EQ(0.5f, u2->time);
    EXPECT_EQ(2, sut.Count());

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_TRUE(u1->IsFinished());
    EXPECT_FALSE(u2->IsFinished());
    EXPECT_EQ(1.0f, u1->time);
    EXPECT_EQ(1.0f, u2->time);
    EXPECT_EQ(1, sut.Count());

    sut.OnUpdate(TimeSlice(1));
    EXPECT_TRUE(u1->IsFinished());
    EXPECT_TRUE(u2->IsFinished());
    EXPECT_EQ(1.0f, u1->time);
    EXPECT_EQ(2.0f, u2->time);
}

TEST(Updatables, PointerDeallocates)
{
    auto u1 = MAKE<TestUpdatable>(1);
    auto u2 = MAKE<TestUpdatable>(2);
    Updatables sut;
    sut.Add(u1);
    sut.Add(u2);

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(2, sut.Count());

    u2.reset();

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_TRUE(u1->IsFinished());
    EXPECT_EQ(1, sut.Count());
}

TEST(Updatables, MakeTimedFunc)
{
    float delta = 0;
    auto sut = Updatables::MakeTimedFunc(3, [&](TimeSlice time) {
        delta += time.delta;
        return FinishType::Continue;
    });

    sut(TimeSlice(1));
    EXPECT_EQ(1, delta);
    sut(TimeSlice(2));
    EXPECT_EQ(3, delta);

    sut(TimeSlice(2));
    EXPECT_EQ(3, delta);
}

TEST(Updatables, MakeTimedFuncFractional)
{
    float delta = 0;
    auto sut = Updatables::MakeTimedFunc(3, [&](TimeSlice time) {
        delta += time.delta;
        return FinishType::Continue;
    });

    sut(TimeSlice(1.5f));
    EXPECT_EQ(1.5f, delta);
    sut(TimeSlice(1.5f));
    EXPECT_EQ(3, delta);

    sut(TimeSlice(2));
    EXPECT_EQ(3, delta);
}

TEST(Updatables, MakeDelayFunc)
{
    float delta = 0;
    auto sut = Updatables::MakeDelayFunc(3, [&](TimeSlice time) {
        delta += time.delta;
        return FinishType::Continue;
    });

    sut(TimeSlice(1));
    EXPECT_EQ(0, delta);
    sut(TimeSlice(2));
    EXPECT_EQ(0, delta);

    sut(TimeSlice(2));
    EXPECT_EQ(2, delta);
}

TEST(Updatables, AddDelay)
{
    auto updatable = MAKE<TestUpdatable>(100);
    Updatables sut;

    EXPECT_EQ(0, updatable->time);

    sut.AddDelay(2, updatable);

    sut.OnUpdate({2});
    EXPECT_EQ(0, updatable->time);

    sut.OnUpdate({1});
    EXPECT_EQ(1, updatable->time);
}

TEST(Updatables, MakeDelayFuncFractional)
{
    float delta = 0;
    auto sut = Updatables::MakeDelayFunc(2.5f, [&](TimeSlice time) {
        delta += time.delta;
        return FinishType::Continue;
    });

    sut(TimeSlice(1));
    EXPECT_EQ(0, delta);
    sut(TimeSlice(1.5f));
    EXPECT_EQ(0, delta);

    sut(TimeSlice(2));
    EXPECT_EQ(2, delta);
}

TEST(Updatables, MakeSpeedFunc)
{
    float delta = 0;
    auto sut = Updatables::MakeSpeedFunc(2, [&](TimeSlice time) {
        delta += time.delta;
        return FinishType::Continue;
    });

    sut(TimeSlice(1));
    EXPECT_EQ(2, delta);
}

TEST(Updatables, AddUpdatableWhileUpdating)
{
    Updatables sut;

    sut.AddContinue([&](auto time) {
        sut.AddContinue([](auto time) {
        });
    });

    sut.OnUpdate(TimeSlice(1));
    EXPECT_EQ(2, sut.Count());
}

TEST(Updatables, RemoveAll)
{
    Updatables sut;

    sut.AddContinue([&](auto time) {
    });
    
    EXPECT_EQ(1, sut.Count());
    sut.RemoveAll();

    EXPECT_EQ(0, sut.Count());
}
