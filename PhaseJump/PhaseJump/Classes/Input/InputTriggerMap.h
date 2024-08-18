#pragma once

#include "SomeEvent.h"
#include "TriggerMap.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/6/24
 */
namespace PJ {
    /// Event sent when an action event occurs based on user input
    class InputActionEvent : public SomeEvent {
    public:
        String action;

        InputActionEvent(String action) :
            action(action) {}
    };

    using InputTriggerMap = TriggerMap<SomeUIEvent, String>;

    class SomeInputActionEventResponder {
    public:
        virtual ~SomeInputActionEventResponder() {}

        virtual void OnInputAction(InputActionEvent const& event) = 0;
    };
} // namespace PJ
