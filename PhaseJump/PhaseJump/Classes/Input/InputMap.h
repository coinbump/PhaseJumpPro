#pragma once

#include "SomeSignal.h"
#include "TriggerMap.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/6/24
 */
namespace PJ {
    /// Event sent when an action event occurs based on user input
    class InputActionEvent : public SomeSignal {
    public:
        String action;

        InputActionEvent(String action) :
            action(action) {}
    };

    /// Stores information for broadcasting signals from input events
    class InputMap {
    public:
        using TriggerMap = PJ::TriggerMap<SomeSignal, String>;
        using FilterFunc = TriggerMap::FilterFunc;

        TriggerMap map;

        void MapControllerButtonDown(String buttonId, String action);
        void MapControllerAxes(String axisX, String axisY, String action);
    };

} // namespace PJ
