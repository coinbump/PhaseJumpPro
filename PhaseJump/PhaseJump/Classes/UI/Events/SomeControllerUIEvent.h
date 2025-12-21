#pragma once

#include "List.h"
#include "SomeUIEvent.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 8/6/24
 */
namespace PJ {
    namespace ControllerButtonId {
        auto constexpr South = "south";
        auto constexpr East = "east";
        auto constexpr West = "west";
        auto constexpr North = "north";
        auto constexpr LeftStick = "left.stick";
        auto constexpr RightStick = "right.stick";
        auto constexpr LeftShoulder = "left.shoulder";
        auto constexpr RightShoulder = "right.shoulder";

        // FUTURE: add more button Ids as needed
    } // namespace ControllerButtonId

    namespace ControllerAxisId {
        auto constexpr LeftX = "left.x";
        auto constexpr LeftY = "left.y";
        auto constexpr RightX = "right.x";
        auto constexpr RightY = "right.y";
        auto constexpr LeftTrigger = "left.trigger";
        auto constexpr RightTrigger = "right.trigger";

        extern VectorList<String> allCases;
    } // namespace ControllerAxisId

    /// An event from an input controller (joystick, gamepad, etc.)
    class SomeControllerUIEvent : public SomeSignal {
    public:
        using Base = SomeControllerUIEvent;

        virtual String ControllerId() const = 0;
    };

    class ControllerAxisUIEvent : public SomeControllerUIEvent {
    public:
        using Base = SomeControllerUIEvent;

        String controllerId;
        String axisId;

        /// Normalized value [-1.0-1.0]
        float value = 0;

        ControllerAxisUIEvent(String controllerId, String axisId, float value) :
            controllerId(controllerId),
            axisId(axisId),
            value(value) {}

        // MARK: SomeControllerUIEvent

        virtual String ControllerId() const override {
            return controllerId;
        }
    };

    /// Controller button event
    class ControllerButtonUIEvent : public SomeControllerUIEvent {
    public:
        using Base = SomeSignal;

        String controllerId;
        String buttonId;
        int buttonIndex{};

        ControllerButtonUIEvent(String controllerId, String buttonId, int buttonIndex) :
            controllerId(controllerId),
            buttonId(buttonId),
            buttonIndex(buttonIndex) {}

        // MARK: SomeControllerUIEvent

        virtual String ControllerId() const override {
            return controllerId;
        }
    };

    /// Controller button down event
    class ControllerButtonDownUIEvent : public ControllerButtonUIEvent {
    public:
        using Base = ControllerButtonUIEvent;

        ControllerButtonDownUIEvent(String controllerId, String buttonId, int buttonIndex) :
            Base(controllerId, buttonId, buttonIndex) {}
    };
} // namespace PJ
