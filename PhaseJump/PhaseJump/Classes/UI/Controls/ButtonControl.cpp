#include "ButtonControl.h"
#include "SomeStateHandler.h"
#include "Utils.h"

using namespace std;
using namespace PJ;

void ButtonControl::SetIsHovering(bool value) {
    isHovering = value;

    if (isTracking) {
        return;
    }

    GUARD(composeTimedStateMachine)
    composeTimedStateMachine->SetState(isHovering ? StateType::Hover : StateType::Normal);
}

ButtonControl::ButtonControl() {}

void ButtonControl::Awake() {
    Base::Awake();

    composeTimedStateMachine = std::make_unique<ComposeTimedStateMachine<StateType>>(*this);
}

void ButtonControl::OnPointerDown(PointerDownUIEvent _event) {
    GUARD(composeTimedStateMachine)

    switch (trackingType) {
    case TrackingType::Immediate:
        OnPress();
        break;
    case TrackingType::Track:
        isTracking = true;
        composeTimedStateMachine->SetState(StateType::Press);
        break;
    }
}

void ButtonControl::OnPointerEnter(PointerEnterUIEvent _event) {
    GUARD(composeTimedStateMachine)

    if (!isTracking) {
        return;
    }
    composeTimedStateMachine->SetState(StateType::Press);
}

void ButtonControl::OnPointerExit(PointerExitUIEvent _event) {
    GUARD(composeTimedStateMachine)

    if (!isTracking) {
        return;
    }
    composeTimedStateMachine->SetState(StateType::Normal);
}

void ButtonControl::OnPointerUp(PointerUpUIEvent _event) {
    GUARD(composeTimedStateMachine)

    if (!isTracking) {
        return;
    }
    isTracking = false;

    bool wasPressed = false;
    switch (composeTimedStateMachine->State()) {
    case StateType::Press:
        composeTimedStateMachine->SetState(isHovering ? StateType::Hover : StateType::Normal);
        wasPressed = true;
        break;
    default:
        composeTimedStateMachine->SetState(StateType::Normal);
        break;
    }

    if (wasPressed) {
        OnPress();
    }
}

void ButtonControl::OnPress() {
    // Debug.Log("Button Pressed");
    auto listener = Listener();
    if (nullptr == listener) {
        return;
    }
    listener->OnEvent(MAKE<PressEvent>(id));
}

void ButtonControl::OnStateChange(TimedStateMachine<StateType>& inStateMachine) {
    GUARD(owner)
    auto owner = this->owner;

    // Forward button state change to state handler components
    //    auto stateHandlers = owner->ChildTypeComponents<SomeStateHandler<StateType>>();
    //    for (auto& stateHandler : stateHandlers) {
    //        stateHandler->OnStateChange(inStateMachine.State());
    //    }
}

void ButtonControl::OnStateFinish(TimedStateMachine<StateType>& inStateMachine) {}
