#include "SDLUIEventsBuilder.h"
#include "DropFilesUIEvent.h"
#include "FilePath.h"
#include "OrderedMap.h"
#include "PointerClickUIEvent.h"
#include "SomeControllerUIEvent.h"
#include "SomeKeyUIEvent.h"
#include "World.h"
#include <iostream>

using namespace std;
using namespace PJ;

PointerInputButtonType PointerInputButtonFromSDLButton(Uint8 sdlButton) {
    auto inputButton = PointerInputButton::Left;
    switch (sdlButton) {
    case SDL_BUTTON_RIGHT:
        inputButton = PointerInputButton::Right;
        break;
    case SDL_BUTTON_MIDDLE:
        inputButton = PointerInputButton::Middle;
        break;
    default:
        break;
    }

    return inputButton;
}

void OnControllerButtonDown(String controllerId, int button, UIEventList& result) {
    OrderedMap<int, String> map = {
        { SDL_GAMEPAD_BUTTON_SOUTH, ControllerButtonId::South },
        { SDL_GAMEPAD_BUTTON_EAST, ControllerButtonId::East },
        { SDL_GAMEPAD_BUTTON_WEST, ControllerButtonId::West },
        { SDL_GAMEPAD_BUTTON_NORTH, ControllerButtonId::North },
        { SDL_GAMEPAD_BUTTON_LEFT_STICK, ControllerButtonId::LeftStick },
        { SDL_GAMEPAD_BUTTON_RIGHT_STICK, ControllerButtonId::RightStick },
        { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, ControllerButtonId::LeftShoulder },
        { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, ControllerButtonId::RightShoulder }
    };

    String buttonId = map[button];
    auto event = MAKE<ControllerButtonDownUIEvent>(controllerId, buttonId, button);
    result.push_back(SCAST<SomeSignal>(event));
}

UIEventList SDLUIEventsBuilder::BuildUIEvents(SDLEventList const& events, float uiScale) {
    UIEventList result;

    VectorList<SP<DropFilesUIEvent>> dropFileEvents;

    for (auto& sdlEvent : events) {
        switch (sdlEvent.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            {
                int32_t width = sdlEvent.window.data1;
                int32_t height = sdlEvent.window.data2;
                auto event = MAKE<WindowResizeUIEvent>(Vector2{ (float)width, (float)height });
                result.push_back(event);
                break;
            }
        // TODO: SDL sends both Joystick and gamepad event for the same event, what to do here?
        //        case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
        //            OnControllerButtonDown(sdlEvent.jbutton.which, sdlEvent.jbutton.button,
        //            result); break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            {
                auto which = sdlEvent.gaxis.which;
                OnControllerButtonDown(MakeString((int)which), sdlEvent.gbutton.button, result);
                break;
            }
        // TODO: this event isn't very useful because it only gets sent when the axis value changes.
        // We need to aggregate and store the values somewhere to be read in update loop
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            {
                auto which = sdlEvent.gaxis.which;
                auto axis = sdlEvent.gaxis.axis;
                auto value = sdlEvent.gaxis.value;

                OrderedMap<SDL_GamepadAxis, String> map = {
                    { SDL_GAMEPAD_AXIS_LEFTX, ControllerAxisId::LeftX },
                    { SDL_GAMEPAD_AXIS_LEFTY, ControllerAxisId::LeftY },
                    { SDL_GAMEPAD_AXIS_RIGHTX, ControllerAxisId::RightX },
                    { SDL_GAMEPAD_AXIS_RIGHTY, ControllerAxisId::RightY },
                    { SDL_GAMEPAD_AXIS_LEFT_TRIGGER, ControllerAxisId::LeftTrigger },
                    { SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, ControllerAxisId::RightTrigger }
                };
                String axisId = map[(SDL_GamepadAxis)axis];

                auto normalValue = value < 0 ? -((float)value / (float)INT16_MIN)
                                             : (float)value / (float)(INT16_MAX);

                PJ::Log(std::format("Axis: {} Value: {}", axisId, normalValue));

                auto event =
                    MAKE<ControllerAxisUIEvent>(MakeString((int)which), axisId, normalValue);
                result.push_back(SCAST<SomeSignal>(event));
                break;
            }
        case SDL_EVENT_PEN_DOWN:
            {
                break;
            }
            // TODO: Pen isn't recognized on Mac
        case SDL_EVENT_PEN_MOTION:
            {
                //                auto pressure = sdlEvent.pmotion.axes[SDL_PEN_AXIS_PRESSURE];
                //                std::cout << pressure;
                break;
            }
        case SDL_EVENT_DROP_FILE:
            {
                // Combine separate drop file events
                auto droppedFile = sdlEvent.drop.data;
                auto event = MAKE<DropFilesUIEvent>(
                    VectorList<FilePath>{ FilePath(droppedFile) },
                    Vector2(sdlEvent.drop.x, sdlEvent.drop.y)
                );
                dropFileEvents.push_back(event);
                break;
            }
        case SDL_EVENT_KEY_DOWN:
            {
                KeyScanCode scanCode(sdlEvent.key.scancode);
                KeyCode keyCode(sdlEvent.key.key);
                KeyModifiers keyModifiers;

                keyModifiers.AddIf(
                    KeyModifier::ShiftLeft, (sdlEvent.key.mod & SDL_KMOD_LSHIFT) != 0
                );
                keyModifiers.AddIf(
                    KeyModifier::ShiftRight, (sdlEvent.key.mod & SDL_KMOD_RSHIFT) != 0
                );
                keyModifiers.AddIf(KeyModifier::Shift, (sdlEvent.key.mod & SDL_KMOD_SHIFT) != 0);
                keyModifiers.AddIf(
                    KeyModifier::ControlLeft, (sdlEvent.key.mod & SDL_KMOD_LCTRL) != 0
                );
                keyModifiers.AddIf(
                    KeyModifier::ControlRight, (sdlEvent.key.mod & SDL_KMOD_RCTRL) != 0
                );
                keyModifiers.AddIf(KeyModifier::Control, (sdlEvent.key.mod & SDL_KMOD_CTRL) != 0);
                keyModifiers.AddIf(KeyModifier::AltLeft, (sdlEvent.key.mod & SDL_KMOD_LALT) != 0);
                keyModifiers.AddIf(KeyModifier::AltRight, (sdlEvent.key.mod & SDL_KMOD_RALT) != 0);
                keyModifiers.AddIf(KeyModifier::Alt, (sdlEvent.key.mod & SDL_KMOD_ALT) != 0);
                keyModifiers.AddIf(KeyModifier::GUILeft, (sdlEvent.key.mod & SDL_KMOD_LGUI) != 0);
                keyModifiers.AddIf(KeyModifier::GUIRight, (sdlEvent.key.mod & SDL_KMOD_RGUI) != 0);
                keyModifiers.AddIf(KeyModifier::GUI, (sdlEvent.key.mod & SDL_KMOD_GUI) != 0);
                keyModifiers.AddIf(KeyModifier::NumLock, (sdlEvent.key.mod & SDL_KMOD_NUM) != 0);
                keyModifiers.AddIf(KeyModifier::CapsLock, (sdlEvent.key.mod & SDL_KMOD_CAPS) != 0);
                keyModifiers.AddIf(
                    KeyModifier::ScrollLock, (sdlEvent.key.mod & SDL_KMOD_SCROLL) != 0
                );

#ifdef _WIN32
                keyModifiers.AddIf(KeyModifier::Shortcut, (sdlEvent.key.mod & SDL_KMOD_CTRL) != 0);
#else
                keyModifiers.AddIf(KeyModifier::Shortcut, (sdlEvent.key.mod & SDL_KMOD_GUI) != 0);
#endif

                auto event = MAKE<KeyDownUIEvent>(KeyUIEventCore{
                    .scanCode = scanCode, .keyCode = keyCode, .keyModifiers = keyModifiers });
                result.push_back(SCAST<SomeSignal>(event));
                break;
            }
        case SDL_EVENT_KEY_UP:
            {
                KeyScanCode scanCode(sdlEvent.key.scancode);
                KeyCode keyCode(sdlEvent.key.key);

                auto event =
                    MAKE<KeyUpUIEvent>(KeyUIEventCore{ .scanCode = scanCode, .keyCode = keyCode });
                result.push_back(SCAST<SomeSignal>(event));
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                auto inputButton = PointerInputButtonFromSDLButton(sdlEvent.button.button);
                auto screenPosition = Vector2(sdlEvent.button.x, sdlEvent.button.y);
                auto event = MAKE<PointerDownUIEvent>(PointerButtonUIEventCore{
                    .screenPos = screenPosition, .button = inputButton });
                result.push_back(SCAST<SomeSignal>(event));
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                auto inputButton = PointerInputButtonFromSDLButton(sdlEvent.button.button);
                result.push_back(MAKE<PointerUpUIEvent>(PointerButtonUIEventCore{
                    .button = inputButton }));
                break;
            }
        case SDL_EVENT_MOUSE_MOTION:
            {
                auto screenPosition = Vector2(sdlEvent.motion.x, sdlEvent.motion.y);
                auto delta = Vector2(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
                result.push_back(MAKE<PointerMoveUIEvent>(screenPosition, delta));
                break;
            }
        }
    }

    if (!IsEmpty(dropFileEvents)) {
        VectorList<FilePath> combinedFilePaths;
        for (auto& dropFileEvent : dropFileEvents) {
            AddRange(combinedFilePaths, dropFileEvent->filePaths);
        }
        auto event = MAKE<DropFilesUIEvent>(combinedFilePaths, (*dropFileEvents.begin())->position);
        result.push_back(event);
    }

    return result;
}
