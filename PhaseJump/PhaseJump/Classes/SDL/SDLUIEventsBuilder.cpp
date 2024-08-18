#include "SDLUIEventsBuilder.h"
#include "DropFilesUIEvent.h"
#include "FilePath.h"
#include "PointerClickUIEvent.h"
#include "SomeControllerUIEvent.h"
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

void OnControllerButtonDown(String controllerId, int button, List<SP<SomeUIEvent>>& result) {
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
    result.Add(SCAST<SomeUIEvent>(event));
}

List<SP<SomeUIEvent>> SDLUIEventsBuilder::BuildUIEvents(SDLEventList const& events) {
    List<SP<SomeUIEvent>> result;

    List<SP<DropFilesUIEvent>> dropFileEvents;

    for (auto& sdlEvent : events) {
        switch (sdlEvent.type) {
        // TODO: SDL sends both Joystick and gamepad event for the same event, what to do here?
        //        case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
        //            OnControllerButtonDown(sdlEvent.jbutton.which, sdlEvent.jbutton.button,
        //            result); break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            {
                auto which = sdlEvent.gaxis.which;
                OnControllerButtonDown(String((int)which), sdlEvent.gbutton.button, result);
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

                cout << "Axis: " << axisId << " Value: " << normalValue << std::endl;

                auto event = MAKE<ControllerAxisUIEvent>(String((int)which), axisId, normalValue);
                result.Add(SCAST<SomeUIEvent>(event));
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
                    List<FilePath>{ FilePath(droppedFile) },
                    ScreenPosition(Vector2(sdlEvent.drop.x, sdlEvent.drop.y))
                );
                dropFileEvents.Add(event);
                break;
            }
        case SDL_EVENT_KEY_DOWN:
            {
                KeyScanCode scanCode(sdlEvent.key.scancode);
                KeyCode keyCode(sdlEvent.key.key);
                // FUTURE: SDL_Keymod mod;

                auto event = MAKE<KeyDownUIEvent>(scanCode, keyCode);
                result.Add(SCAST<SomeUIEvent>(event));
                break;
            }
        case SDL_EVENT_KEY_UP:
            {
                KeyScanCode scanCode(sdlEvent.key.scancode);
                KeyCode keyCode(sdlEvent.key.key);
                // FUTURE: SDL_Keymod mod;

                auto event = MAKE<KeyUpUIEvent>(scanCode, keyCode);
                result.Add(SCAST<SomeUIEvent>(event));
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                auto inputButton = PointerInputButtonFromSDLButton(sdlEvent.button.button);
                auto screenPosition = ScreenPosition(Vector2(sdlEvent.button.x, sdlEvent.button.y));
                auto event = MAKE<PointerDownUIEvent>(screenPosition, inputButton);
                result.Add(SCAST<SomeUIEvent>(event));
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                auto inputButton = PointerInputButtonFromSDLButton(sdlEvent.button.button);
                result.Add(MAKE<PointerUpUIEvent>(inputButton));
                break;
            }
        case SDL_EVENT_MOUSE_MOTION:
            {
                // TODO: support which
                /* Used as the device ID for mouse events simulated with touch input */
                //                #define SDL_TOUCH_MOUSEID ((SDL_MouseID)-1)
                //
                //                /* Used as the SDL_TouchID for touch events simulated with mouse
                //                input */ #define SDL_MOUSE_TOUCHID ((SDL_TouchID)-1)

                auto screenPosition = ScreenPosition(Vector2(sdlEvent.motion.x, sdlEvent.motion.y));
                auto delta = Vector2(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
                result.Add(MAKE<PointerMoveUIEvent>(screenPosition, delta));
                break;
            }
        }
    }

    if (!IsEmpty(dropFileEvents)) {
        List<FilePath> combinedFilePaths;
        for (auto& dropFileEvent : dropFileEvents) {
            AddRange(combinedFilePaths, dropFileEvent->filePaths);
        }
        auto event = MAKE<DropFilesUIEvent>(combinedFilePaths, (*dropFileEvents.begin())->position);
        result.Add(SCAST<SomeUIEvent>(event));
    }

    return result;
}
