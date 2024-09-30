#include "Valve.h"

using namespace std;
using namespace PJ;

Valve::Valve(bool isOn) :
    state(isOn ? StateType::On : StateType::Off) {
    onUpdateFunc = [](auto& updatable, TimeSlice time) {
        This& valve = *(static_cast<This*>(&updatable));
        GUARDR(valve.timer, FinishType::Continue);

        valve.timer->OnUpdate(time);

        if (valve.timer->IsFinished()) {
            valve.timer.reset();

            switch (valve.state) {
            case StateType::TurnOn:
                valve.SetState(StateType::On);
                break;
            case StateType::TurnOff:
                valve.SetState(StateType::Off);
                break;
            default:
                PJ::Log("ERROR. Animating valve with no destination");
                break;
            }
        }

        // Even though valve state hasn't changed, the time state has
        valve.OnValveUpdate();

        return FinishType::Continue;
    };
}

float Valve::Value() const {
    switch (state) {
    case StateType::On:
        return 1;
    case StateType::Off:
        return 0;
    case StateType::TurnOn:
        GUARDR(timer, 1)
        return timer->Progress();
    case StateType::TurnOff:
        GUARDR(timer, 0)
        return 1.0f - timer->Progress();
    }
}

/**
 Turn on the valve in N seconds
 If we are interrupting a previous animation, the actual duration will be proportional to
 the distance left
 */
void Valve::TurnOn(float duration) {
    GUARD(!IsOn())

    if (duration <= 0) {
        SetState(StateType::On);
        timer.reset();
        return;
    }

    auto value = Value();

    SetState(StateType::TurnOn);
    timer = MAKE<Timer>(duration, Runner::RunType::Once);
    timer->SetProgress(value);
}

/**
 Turn on the valve in N seconds
 If we are interrupting a previous animation, the actual duration will be proportional to
 the distance left
 */
void Valve::TurnOff(float duration) {
    GUARD(!IsOff())

    if (duration <= 0) {
        SetState(StateType::Off);
        timer.reset();
        return;
    }

    auto value = Value();

    SetState(StateType::TurnOff);
    timer = MAKE<Timer>(duration, Runner::RunType::Once);
    timer->SetProgress(1.0f - value);
}

void Valve::SetState(StateType value) {
    GUARD(state != value)
    state = value;

    OnValveUpdate();
}

void Valve::OnValveUpdate() {
    GUARD(onValveUpdateFunc)
    onValveUpdateFunc(*this);
}
