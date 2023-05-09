#include "ButtonControl.h"
#include "SomeStateHandler.h"

using namespace std;
using namespace PJ;

void ButtonControl::SetIsHovering(bool value)
{
    if (isTracking) { return; }
    isHovering = value;
    core->SetState(isHovering ? StateType::Hover : StateType::Normal);
}

ButtonControl::ButtonControl()
{
}

void ButtonControl::Awake() {
    Base::Awake();

    auto sharedThis = SCAST<ButtonControl>(shared_from_this());
    core->Configure(SCAST<SomeGoStateListener<ButtonControlStateType>>(sharedThis));
}

void ButtonControl::OnPointerDownEvent(PointerDownUIEvent<LocalPosition> _event)
{
    switch (trackingType)
    {
        case TrackingType::Immediate:
            OnPress();
            break;
        case TrackingType::Track:
            isTracking = true;
            core->SetState(StateType::Press);
            break;
    }
}

void ButtonControl::OnPointerEnterEvent(PointerEnterUIEvent _event)
{
    if (!isTracking) { return; }
    core->SetState(StateType::Press);
}

void ButtonControl::OnPointerExitEvent(PointerExitUIEvent _event)
{
    if (!isTracking) { return; }
    core->SetState(StateType::Normal);
}

void ButtonControl::OnPointerUpEvent(PointerUpUIEvent _event)
{
    if (!isTracking) { return; }
    isTracking = false;

    bool wasPressed = false;
    switch (core->State())
    {
        case StateType::Press:
            core->SetState(isHovering ? StateType::Hover : StateType::Normal);
            wasPressed = true;
            break;
        default:
            core->SetState(StateType::Normal);
            break;
    }

    if (wasPressed)
    {
        OnPress();
    }
}

void ButtonControl::OnPress()
{
    // Debug.Log("Button Pressed");
    auto listener = Listener();
    if (nullptr == listener) { return; }
    listener->OnEvent(MAKE<PressEvent>(id));
}

void ButtonControl::OnStateChange(GoStateMachine<StateType>& inStateMachine)
{
    auto owner = this->owner.lock();

    // Forward button state change to state handler components
    auto stateHandlers = owner->TypeComponentsInChildren<SomeStateHandler<StateType>>();
    for (auto stateHandler : stateHandlers)
    {
        stateHandler->OnStateChange(inStateMachine.State());
    }
}

void ButtonControl::OnStateFinish(GoStateMachine<StateType>& inStateMachine)
{
}
