#include "gtest/gtest.h"
#include "Valve.h"
#include "SomeInterpolate.h"

using namespace PJ;
using namespace std;

namespace ValveTests {
    class TestValve : public Valve
    {
    public:
        TestValve(std::optional<float> onOffDuration)
        {
            turnOnTimer = MAKE<TransformTimer>(1.0f, SomeRunner::RunType::RunOnce, MAKE<InterpolateLinear>());
            turnOffTimer = MAKE<TransformTimer>(1.0f, SomeRunner::RunType::RunOnce, MAKE<InterpolateLinear>());
            this->SetOnStateDuration(onOffDuration);
            this->SetOffStateDuration(onOffDuration);
        }
    };
}

using namespace ValveTests;

TEST(Valve, WithoutOnOffDuration)
{
    auto sut = MAKE<TestValve>(std::nullopt);
    sut->Go();
    EXPECT_EQ(Valve::StateType::Off, sut->State());
    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(Valve::StateType::Off, sut->State());

    sut->TurnOff(InputEffect::Immediate);
    EXPECT_EQ(Valve::StateType::Off, sut->State());
    EXPECT_EQ(0, sut->ValveState());

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(Valve::StateType::Off, sut->State());

    sut->TurnOn(InputEffect::Timed);
    sut->OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(.5f, sut->ValveState());
    EXPECT_EQ(Valve::StateType::TurnOn, sut->State());
    sut->OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(Valve::StateType::On, sut->State());

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(Valve::StateType::On, sut->State());

    sut->OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(Valve::StateType::On, sut->State());

    sut->TurnOff(InputEffect::Timed);
    EXPECT_EQ(Valve::StateType::TurnOff, sut->State());
    EXPECT_EQ(1.0f, sut->ValveState());

    sut->OnUpdate(TimeSlice(0.3f));
    EXPECT_EQ(.7f, sut->ValveState());
    EXPECT_EQ(Valve::StateType::TurnOff, sut->State());

    sut->TurnOff(InputEffect::Immediate);
    EXPECT_EQ(Valve::StateType::Off, sut->State());
    EXPECT_EQ(0, sut->ValveState());
}

TEST(Valve, WithOnOffDuration)
{
    auto sut = MAKE<TestValve>(std::optional<float>(1.0f));
    sut->Go();
    EXPECT_EQ(Valve::StateType::Off, sut->State());
    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(Valve::StateType::TurnOn, sut->State());

    sut->TurnOff(InputEffect::Immediate);
    EXPECT_EQ(Valve::StateType::Off, sut->State());
    EXPECT_EQ(0, sut->ValveState());

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(Valve::StateType::TurnOn, sut->State());

    sut->OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(.5f, sut->ValveState());
    EXPECT_EQ(Valve::StateType::TurnOn, sut->State());
    sut->OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(Valve::StateType::On, sut->State());

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(Valve::StateType::TurnOff, sut->State());

    sut->OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(.5f, sut->ValveState());
    sut->OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(Valve::StateType::Off, sut->State());

    sut->SetOffStateDuration(std::optional<float>(-1.0f));
    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(Valve::StateType::Off, sut->State());

    sut->SetOffStateDuration(std::optional<float>(1.0f));
    sut->TurnOn(InputEffect::Timed);
    sut->OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(Valve::StateType::TurnOn, sut->State());

    sut->TurnOff(InputEffect::Timed);
    EXPECT_EQ(Valve::StateType::TurnOff, sut->State());
    EXPECT_EQ(.7f, sut->ValveState());

    sut->TurnOff(InputEffect::Immediate);
    EXPECT_EQ(Valve::StateType::Off, sut->State());
    EXPECT_EQ(0, sut->ValveState());
}
