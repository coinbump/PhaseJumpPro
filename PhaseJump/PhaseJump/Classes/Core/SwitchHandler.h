#pragma once

#include "Switchable.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/16/24
 */
namespace PJ {
    /// Component with a switch
    class SwitchHandler : public WorldComponent<>, public Switchable {
    protected:
        SwitchState state = SwitchState::Off;

    public:
        using OnSwitchChangeFunc = std::function<void(SwitchHandler&)>;

        OnSwitchChangeFunc onSwitchChangeFunc;

        SwitchHandler(SwitchState state = SwitchState::Off) :
            state(state) {}

        SwitchHandler(OnSwitchChangeFunc onSwitchChangeFunc) :
            onSwitchChangeFunc(onSwitchChangeFunc) {}

        // MARK: Switchable

        bool IsOn() const override {
            return state == SwitchState::On;
        }

        void SetIsOn(bool value) override {
            GUARD(IsOn() != value)

            state = value ? SwitchState::On : SwitchState::Off;
            OnSwitchChange();
        }

        void Toggle() {
            SetIsOn(!IsOn());
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SwitchHandler";
        }

    protected:
        // MARK: Switchable

        void OnSwitchChange() override {
            GUARD(onSwitchChangeFunc)
            onSwitchChangeFunc(*this);
        }
    };
} // namespace PJ
