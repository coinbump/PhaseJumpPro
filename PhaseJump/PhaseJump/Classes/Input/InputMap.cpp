#include "InputMap.h"
#include "SomeControllerUIEvent.h"

using namespace std;
using namespace PJ;

void InputMap::MapControllerButtonDown(String buttonId, String action) {
    map.AddTrigger(action, [=](SomeUIEvent const& _event) {
        auto event = As<ControllerButtonDownUIEvent>(&_event);
        GUARDR(event, false)

        return event->buttonId == buttonId;
    });
}

void InputMap::MapControllerAxes(String axisX, String axisY, String action) {
    map.AddTrigger(action + ".x", [axisX](SomeUIEvent const& _event) {
        auto event = As<ControllerAxisUIEvent>(&_event);
        GUARDR(event, false)

        return event->axisId == axisX;
    });
    map.AddTrigger(action + ".y", [axisY](SomeUIEvent const& _event) {
        auto event = As<ControllerAxisUIEvent>(&_event);
        GUARDR(event, false)

        return event->axisId == axisY;
    });
}
