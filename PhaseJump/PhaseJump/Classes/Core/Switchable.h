#pragma once

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 8/21/24
 */
namespace PJ {
    enum class SwitchState { Off, On };

    /// Can be turned on or off
    class Switchable {
    public:
        virtual ~Switchable() {}

        virtual bool IsOn() const = 0;
        virtual void SetIsOn(bool value) = 0;
        virtual void OnSwitchChange() = 0;

        void Toggle() {
            SetIsOn(!IsOn());
        }
    };
} // namespace PJ
