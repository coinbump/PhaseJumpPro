#include "gtest/gtest.h"
#include "TimedSequence.h"
#include "Timer.h"

using namespace PJ;
using namespace std;

namespace TimedSequenceTests {
}

using namespace TimedSequenceTests;

TEST(TimedSequence, Tests)
{
    TimedSequence sut;
    auto timer1 = make_shared<Timer>(1.0f, SomeRunner::RunType::RunOnce);
    auto timer2 = make_shared<Timer>(2.0f, SomeRunner::RunType::RunOnce);
    sut.Add(timer1);
    sut.Add(timer2);

    EXPECT_FALSE(sut.IsFinished());

    sut.OnUpdate(TimeSlice(0.5f));

    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(0.5f, timer1->Progress());

    sut.OnUpdate(TimeSlice(1.5f));

    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(1.0f, timer1->Progress());
    EXPECT_TRUE(timer1->IsFinished());
    EXPECT_EQ(0.5f, timer2->Progress());

    sut.OnUpdate(TimeSlice(2.0f));
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_TRUE(timer2->IsFinished());
    EXPECT_EQ(1.0f, timer2->Progress());
}
