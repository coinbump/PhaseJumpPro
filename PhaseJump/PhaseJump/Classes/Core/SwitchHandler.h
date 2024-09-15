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
        bool isOn = false;

    public:
        using OnSwitchChangeFunc = std::function<void(SwitchHandler&)>;

        OnSwitchChangeFunc onSwitchChangeFunc;

        SwitchHandler() {}

        SwitchHandler(OnSwitchChangeFunc onSwitchChangeFunc) :
            onSwitchChangeFunc(onSwitchChangeFunc) {}

        bool IsOn() const {
            return isOn;
        }

        void SetIsOn(bool value) {
            GUARD(isOn != value)

            isOn = value;
            OnSwitchChange();
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SwitchHandler";
        }

    protected:
        virtual void OnSwitchChange() {
            GUARD(onSwitchChangeFunc)
            onSwitchChangeFunc(*this);
        }
    };
} // namespace PJ
