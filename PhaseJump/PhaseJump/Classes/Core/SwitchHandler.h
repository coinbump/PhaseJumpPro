#ifndef PJSWITCHHANDLER_H
#define PJSWITCHHANDLER_H

#include "WorldComponent.h"
#include "Switchable.h"

/*
 RATING: 5 stars
 Simple component
 CODE REVIEW: 12/26/22
 */
namespace PJ
{
    class SwitchHandler : public WorldComponent, public Switchable
    {
    protected:
        bool isOn = false;

    public:
        bool IsOn() const { return isOn; }
        void SetIsOn(bool value) {
            if (isOn == value)
            {
                return;
            }

            isOn = value;
            OnSwitchChange();
        }

    protected:
        virtual void OnSwitchChange() { }

        virtual void OnValidate()
        {
            OnSwitchChange();
        }
    };

    /// A composable switch handler that notifies its owner
    template <class Owner>
    class ComposeSwitchHandler : public PJ::SwitchHandler {
    public:
        Owner* owner;

        using Base = PJ::SwitchHandler;

        ComposeSwitchHandler(Owner* owner) : owner(owner) {
        }

        void OnSwitchChange() override {
            Base::OnSwitchChange();

            owner->OnSwitchChange();
        }
    };
}

#endif
