#include "gtest/gtest.h"
#include "World.h"
#include "WorldNode.h"
#include "Driver.h"

using namespace PJ;
using namespace std;

namespace SomeDriverTests {
}

using namespace SomeDriverTests;

TEST(TimerDriver, RunOnce)
{
    int i = 0;
    TimerDriver driver(3, RunType::Once, [&]() {
        i++;
    });
    EXPECT_EQ(0, i);
    driver.OnUpdate(TimeSlice(1));
    EXPECT_EQ(0, i);

    driver.OnUpdate(TimeSlice(2));
    EXPECT_EQ(1, i);
    
    driver.OnUpdate(TimeSlice(20));
    EXPECT_EQ(1, i);
}

TEST(TimerDriver, RunRepeat)
{
    int i = 0;
    TimerDriver driver(1, RunType::Repeat, [&]() {
        i++;
    });
    EXPECT_EQ(0, i);

    // FUTURE: Timers don't work when you send it time longer than their duration. Should they?
    driver.OnUpdate(TimeSlice(1));
    EXPECT_EQ(1, i);
    driver.OnUpdate(TimeSlice(1));
    EXPECT_EQ(2, i);
    driver.OnUpdate(TimeSlice(1));
    EXPECT_EQ(3, i);
}
