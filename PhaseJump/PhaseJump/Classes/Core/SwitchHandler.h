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
}

#endif
