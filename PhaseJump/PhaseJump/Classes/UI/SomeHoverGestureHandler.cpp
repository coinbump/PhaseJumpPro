#include "SomeHoverGestureHandler.h"

using namespace std;
using namespace PJ;

SomeHoverGestureHandler::SomeHoverGestureHandler() {
    AddSignalHandler<HoverUIEvent>({ .id = SignalId::Hover, .func = [](auto& owner, auto& event) {
                                        static_cast<This*>(&owner)->SetIsHovering(event.isHovering);
                                    } });
}

// MARK: - ValveHoverGestureHandler

ValveHoverGestureHandler::ValveHoverGestureHandler(float turnOnDuration, float turnOffDuration) :
    turnOnDuration(turnOnDuration),
    turnOffDuration(turnOffDuration) {
    GetUpdatable().onUpdateFunc = [this](auto& updatable, auto time) {
        valve.OnUpdate(time);
        return FinishType::Continue;
    };
}

void ValveHoverGestureHandler::SetIsHovering(bool value) {
    Base::SetIsHovering(value);

    if (value) {
        valve.TurnOn(turnOnDuration);
    } else {
        valve.TurnOff(turnOffDuration);
    }
}
