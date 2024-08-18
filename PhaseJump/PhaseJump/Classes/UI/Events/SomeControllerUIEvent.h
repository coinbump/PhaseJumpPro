#pragma once

#include "List.h"
#include "SomeUIEvent.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/6/24
 */
namespace PJ {
    namespace ControllerButtonId {
        static auto const South = "south";
        static auto const East = "east";
        static auto const West = "west";
        static auto const North = "north";
        static auto const LeftStick = "left.stick";
        static auto const RightStick = "right.stick";
        static auto const LeftShoulder = "left.shoulder";
        static auto const RightShoulder = "right.shoulder";

        // FUTURE: add more button Ids as needed
    } // namespace ControllerButtonId

    namespace ControllerAxisId {
        static auto const LeftX = "left.x";
        static auto const LeftY = "left.y";
        static auto const RightX = "right.x";
        static auto const RightY = "right.y";
        static auto const LeftTrigger = "left.trigger";
        static auto const RightTrigger = "right.trigger";

        extern List<String> allCases;
    } // namespace ControllerAxisId

    /// An event from an input controller (joystick, gamepad, etc.)
    class SomeControllerUIEvent : public SomeUIEvent {
    public:
        String controllerId;

        SomeControllerUIEvent(String controllerId) :
            controllerId(controllerId) {}
    };

    class ControllerAxisUIEvent : public SomeControllerUIEvent {
    public:
        using Base = SomeControllerUIEvent;

        String axisId;

        /// Normalized value [-1.0-1.0]
        float value = 0;

        ControllerAxisUIEvent(String controllerId, String axisId, float value) :
            Base(controllerId),
            axisId(axisId),
            value(value) {}
    };

    /// Controller button event
    class ControllerButtonUIEvent : public SomeControllerUIEvent {
    public:
        using Base = SomeControllerUIEvent;

        String buttonId;
        int buttonIndex = 0;

        ControllerButtonUIEvent(String controllerId, String buttonId, int buttonIndex) :
            Base(controllerId),
            buttonId(buttonId),
            buttonIndex(buttonIndex) {}
    };

    /// Controller button down event
    class ControllerButtonDownUIEvent : public ControllerButtonUIEvent {
    public:
        using Base = ControllerButtonUIEvent;

        ControllerButtonDownUIEvent(String controllerId, String buttonId, int buttonIndex) :
            Base(controllerId, buttonId, buttonIndex) {}
    };
} // namespace PJ
