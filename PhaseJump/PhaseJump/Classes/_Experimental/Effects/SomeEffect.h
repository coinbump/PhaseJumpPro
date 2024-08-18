#ifndef PJSOMEEFFECT_H
#define PJSOMEEFFECT_H

#include "SwitchHandler.h"

/*
 RATING: 5 stars
 Simple abstract
 CODE REVIEW: 5/10/23
 */
namespace PJ {
    /// A effect is applied when activated
    class SomeEffect : public SwitchHandler {
    public:
        using Base = SwitchHandler;

        void OnSwitchChange() override {
            Base::OnSwitchChange();

            UpdateEffectProperties();
        }

        // Override to update the effect properties when the effect state
        // changes
        virtual void UpdateEffectProperties() {}
    };
} // namespace PJ

#endif
