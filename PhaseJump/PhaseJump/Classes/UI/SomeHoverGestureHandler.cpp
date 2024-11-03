#include "SomeHoverGestureHandler.h"

using namespace std;
using namespace PJ;

SomeHoverGestureHandler::SomeHoverGestureHandler() {
    signalFuncs[SignalId::Hover] = [](auto& owner, auto& signal) {
        auto event = static_cast<HoverUIEvent const*>(&signal);
        static_cast<This*>(&owner)->SetIsHovering(event->isHovering);
    };
}

// MARK: - ValveHoverGestureHandler

ValveHoverGestureHandler::ValveHoverGestureHandler(float turnOnDuration, float turnOffDuration) :
    turnOnDuration(turnOnDuration),
    turnOffDuration(turnOffDuration) {
    updatables.Add(valve);
}

void ValveHoverGestureHandler::SetIsHovering(bool value) {
    Base::SetIsHovering(value);

    GUARD(valve);

    if (value) {
        valve->TurnOn(turnOnDuration);
    } else {
        valve->TurnOff(turnOffDuration);
    }
}
