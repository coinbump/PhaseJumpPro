#ifndef PJTOGGLEBUTTONCONTROL_H
#define PJTOGGLEBUTTONCONTROL_H

#include "ButtonControl.h"
#include "SomeEffect.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    /// A button that, when pressed, toggles off and on
    class ToggleButtonControl : public ButtonControl {
    public:
        using Base = ButtonControl;

        SP<SomeEffect> toggleEffect;

        bool IsToggleOn() const {
            return isToogleOn;
        }

        void SetIsToggleOn(bool value) {
            if (isToogleOn == value) {
                return;
            }
            isToogleOn = value;
            OnToggleChange();
        }

    protected:
        bool isToogleOn = false;

        void Awake() override {
            Base::Awake();

            UpdateToggle();
        }

        void OnPress() override {
            Base::OnPress();

            SetIsToggleOn(!IsToggleOn());
        }

        void OnToggleChange() {
            UpdateToggle();
        }

        virtual void UpdateToggle() {
            if (!toggleEffect) {
                return;
            }
            toggleEffect->SetIsOn(isToogleOn);
        }
    };
} // namespace PJ

#endif
