#include "gtest/gtest.h"
#include "Valve.h"

using namespace PJ;
using namespace std;

namespace ValveTests {
    class TestValve : public Valve
    {
    public:
        TestValve(bool isOn, float& progress) : Valve(isOn) {
            SetOnValveUpdateFunc([&](auto& valve) {
                progress = valve.Value();
            });
        }
    };
}

using namespace ValveTests;

TEST(Valve, Init)
{
    {
        float progress{};
        TestValve sut(true, progress);
        EXPECT_TRUE(sut.IsOn());
        EXPECT_EQ(1, progress);
    }
    {
        float progress{};
        TestValve sut(false, progress);
        EXPECT_FALSE(sut.IsOn());
        EXPECT_EQ(0, progress);
    }
}

TEST(Valve, TurnOnWhenOff)
{
    float progress{};
    TestValve sut(false, progress);
    EXPECT_FALSE(sut.IsOn());

    sut.TurnOn(2);
    EXPECT_EQ(Valve::StateType::TurnOn, sut.State());
    EXPECT_EQ(0, sut.Value());

    sut.OnUpdate({1});
    EXPECT_EQ(0.5f, sut.Value());
    EXPECT_EQ(0.5f, progress);

    sut.OnUpdate({1});
    EXPECT_EQ(1.0f, sut.Value());
    EXPECT_EQ(1.0f, progress);

    EXPECT_TRUE(sut.IsOn());
}

TEST(Valve, TurnOnInterruptTurnOff)
{
    Valve sut(false);
    EXPECT_FALSE(sut.IsOn());

    sut.TurnOn(2);
    EXPECT_EQ(Valve::StateType::TurnOn, sut.State());
    EXPECT_EQ(0, sut.Value());

    sut.OnUpdate({.5f});
    EXPECT_EQ(0.25f, sut.Value());

    sut.TurnOff(2);
    EXPECT_EQ(Valve::StateType::TurnOff, sut.State());
    EXPECT_EQ(0.25f, sut.Value());

    sut.OnUpdate({.5f});
    EXPECT_EQ(0, sut.Value());
    EXPECT_TRUE(sut.IsOff());
}

TEST(Valve, TurnOnWhenOn)
{
    Valve sut(true);
    EXPECT_TRUE(sut.IsOn());

    sut.TurnOn(2);
    EXPECT_EQ(1.0f, sut.Value());
    EXPECT_TRUE(sut.IsOn());
}

TEST(Valve, TurnOffWhenOn)
{
    Valve sut(true);
    EXPECT_FALSE(sut.IsOff());

    sut.TurnOff(2);
    EXPECT_EQ(Valve::StateType::TurnOff, sut.State());
    EXPECT_EQ(1, sut.Value());

    sut.OnUpdate({0.5f});
    EXPECT_EQ(0.75f, sut.Value());

    sut.OnUpdate({1.5f});
    EXPECT_EQ(0, sut.Value());

    EXPECT_TRUE(sut.IsOff());
}

TEST(Valve, TurnOffWhenOff)
{
    Valve sut(false);
    EXPECT_TRUE(sut.IsOff());

    sut.TurnOff(2);
    EXPECT_EQ(0, sut.Value());
    EXPECT_TRUE(sut.IsOff());
}

TEST(Valve, TurnOffInterruptTurnOn)
{
    Valve sut(true);
    EXPECT_TRUE(sut.IsOn());

    sut.TurnOff(2);
    EXPECT_EQ(Valve::StateType::TurnOff, sut.State());
    EXPECT_EQ(1, sut.Value());

    sut.OnUpdate({.5f});
    EXPECT_EQ(0.75f, sut.Value());

    sut.TurnOn(2);
    EXPECT_EQ(Valve::StateType::TurnOn, sut.State());
    EXPECT_EQ(0.75f, sut.Value());

    sut.OnUpdate({.5f});
    EXPECT_EQ(1, sut.Value());
    EXPECT_TRUE(sut.IsOn());
}

TEST(Valve, TurnOnImmediate)
{
    float progress{};
    TestValve sut(false, progress);
    EXPECT_FALSE(sut.IsOn());

    sut.TurnOn(0);
    EXPECT_EQ(Valve::StateType::On, sut.State());
    EXPECT_EQ(1, sut.Value());
    EXPECT_EQ(1, progress);
}

TEST(Valve, TurnOffImmediate)
{
    float progress{};
    TestValve sut(true, progress);
    EXPECT_TRUE(sut.IsOn());

    sut.TurnOff(0);
    EXPECT_EQ(Valve::StateType::Off, sut.State());
    EXPECT_EQ(0, sut.Value());
    EXPECT_EQ(0, progress);
}
