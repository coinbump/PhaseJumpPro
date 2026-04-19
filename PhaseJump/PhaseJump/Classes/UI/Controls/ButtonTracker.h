#pragma once

#include <functional>

/*
 RATING: 5+ stars
 Has unit tests
 CODE REVIEW: 4/16/26
 */
namespace PJ {
    /// Encapsulates pointer tracking and press detection for button-like controls.
    class ButtonTracker {
    public:
        using This = ButtonTracker;

        enum class TrackType {
            // Track the button: press fires on pointer up while still over the control
            Track,

            // Fire the button press on tap (pointer down)
            Immediate
        };

        using OnPressFunc = std::function<void()>;
        using OnStateChangeFunc = std::function<void()>;

        TrackType trackType = TrackType::Track;

        struct Config {
            TrackType trackType = TrackType::Track;
        };

        ButtonTracker(Config const& config) :
            trackType(config.trackType) {}

        ButtonTracker() :
            ButtonTracker(Config{}) {}

        /// Called when the button is pressed
        OnPressFunc onPressFunc;

        /// Called when isPressed changes (owner should call OnControlChange)
        OnStateChangeFunc onStateChangeFunc;

        bool IsTracking() const {
            return isTracking;
        }

        bool IsPressed() const {
            return isPressed;
        }

        void OnPointerDown();
        void OnPointerEnter();
        void OnPointerExit();
        void OnPointerUp();

    protected:
        bool isTracking{};
        bool isPressed{};

        void SetIsPressed(bool value);
    };
} // namespace PJ
