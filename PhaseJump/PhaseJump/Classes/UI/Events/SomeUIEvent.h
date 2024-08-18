#pragma once

#include "SomeEvent.h"
#include "SomePosition.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// A user input event object
    class SomeUIEvent : public SomeEvent {
    public:
        /// Metadata for event
        Tags tags;

        virtual ~SomeUIEvent() {}
    };

    // TODO: move these
    struct KeyScanCode {
        int value;

        KeyScanCode(int value) :
            value(value) {}
    };

    struct KeyCode {
        int value;

        KeyCode(int value) :
            value(value) {}
    };

    /// Key related UI event
    class SomeKeyUIEvent : public SomeUIEvent {
    public:
        /// Scan code of the hardware key that was pressed
        KeyScanCode scanCode;

        /// Key code of the character that was pressed
        KeyCode keyCode;

        SomeKeyUIEvent(KeyScanCode scanCode, KeyCode keyCode) :
            scanCode(scanCode),
            keyCode(keyCode) {}
    };

    /// Key down UI event
    class KeyDownUIEvent : public SomeKeyUIEvent {
    public:
        using Base = SomeKeyUIEvent;

        KeyDownUIEvent(KeyScanCode scanCode, KeyCode keyCode) :
            Base(scanCode, keyCode) {}
    };

    /// Key down UI event
    class KeyUpUIEvent : public SomeKeyUIEvent {
    public:
        using Base = SomeKeyUIEvent;

        KeyUpUIEvent(KeyScanCode scanCode, KeyCode keyCode) :
            Base(scanCode, keyCode) {}
    };

    /// Responds to pointer events
    class SomeKeyUIEventsResponder {
    public:
        virtual ~SomeKeyUIEventsResponder() {}

        // TODO: use const& here?
        virtual void OnKeyDown(KeyDownUIEvent event) {}

        virtual void OnKeyUpEvent(KeyUpUIEvent event) {}
    };

} // namespace PJ
