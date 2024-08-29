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
        bool isFinished = false;

        TestUpdatable(float finishTime) : finishTime(finishTime) {
        }

        void OnUpdate(TimeSlice time) override {
            this->time += time.delta;
            isFinished = this->time >= finishTime;
        }

        bool IsFinished() const override {
            return isFinished;
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
    
    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_TRUE(u1->IsFinished());
    EXPECT_FALSE(u2->IsFinished());
    EXPECT_EQ(1.0f, u1->time);
    EXPECT_EQ(1.0f, u2->time);

    sut.OnUpdate(TimeSlice(1));
    EXPECT_TRUE(u1->IsFinished());
    EXPECT_TRUE(u2->IsFinished());
    EXPECT_EQ(1.0f, u1->time);
    EXPECT_EQ(2.0f, u2->time);
}
