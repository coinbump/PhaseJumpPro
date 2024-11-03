#pragma once

#include "List.h"
#include "SomePosition.h"
#include "SomeSignal.h"
#include "Tags.h"
#include "UnorderedSet.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// A user input event object
    class SomeUIEvent : public SomeSignal {
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

    namespace KeyModifier {
        // Shift key modifiers
        auto constexpr ShiftLeft = "shift.left";
        auto constexpr ShiftRight = "shift.right";
        auto constexpr Shift = "shift";

        // Ctrl/Control key modifiers
        auto constexpr ControlLeft = "control.left";
        auto constexpr ControlRight = "control.right";
        auto constexpr Control = "control";

        // Alt/Option modifiers
        auto constexpr AltLeft = "alt.left";
        auto constexpr AltRight = "alt.right";
        auto constexpr Alt = "alt";

        // GUI key modifiers (Windows: Windows Key, Mac: Command)
        auto constexpr GUILeft = "gui.left";
        auto constexpr GUIRight = "gui.right";
        auto constexpr GUI = "gui";

        // Shortcut key modifier (Windows: Ctrl, Mac: Command)
        auto constexpr Shortcut = "shortcut";

        // Other modifiers
        auto constexpr CapsLock = "caps";
        auto constexpr NumLock = "numLock";
        auto constexpr ScrollLock = "scrollLock";
    } // namespace KeyModifier

    struct KeyModifiers {
        UnorderedSet<String> modifiers;

        void AddIf(String modifier, bool check) {
            GUARD(check)
            modifiers.insert(modifier);
        }
    };

    /// Key related UI event
    class SomeKeyUIEvent : public SomeUIEvent {
    public:
        /// Scan code of the hardware key that was pressed
        KeyScanCode scanCode;

        /// Key code of the character that was pressed
        KeyCode keyCode;

        /// Modifiers pressed along with the key
        KeyModifiers keyModifiers;

        SomeKeyUIEvent(KeyScanCode scanCode, KeyCode keyCode, KeyModifiers keyModifiers) :
            scanCode(scanCode),
            keyCode(keyCode),
            keyModifiers(keyModifiers) {}
    };

    /// Key down UI event
    class KeyDownUIEvent : public SomeKeyUIEvent {
    public:
        using Base = SomeKeyUIEvent;

        KeyDownUIEvent(KeyScanCode scanCode, KeyCode keyCode, KeyModifiers keyModifiers) :
            Base(scanCode, keyCode, keyModifiers) {}
    };

    /// Key down UI event
    class KeyUpUIEvent : public SomeKeyUIEvent {
    public:
        using Base = SomeKeyUIEvent;

        KeyUpUIEvent(KeyScanCode scanCode, KeyCode keyCode) :
            Base(scanCode, keyCode, {}) {}
    };

    // TODO: use UP/VectorList here?
    using UIEventList = List<SP<SomeUIEvent>>;
} // namespace PJ
