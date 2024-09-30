#pragma once

#include "SwitchHandler.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/7/24
 */
namespace PJ {
    /// A effect is applied when activated
    class SomeEffect : public SwitchHandler {
    public:
        using Base = SwitchHandler;

        SomeEffect(SwitchState state = SwitchState::Off) :
            Base(state) {
            onSwitchChangeFunc = [](auto& handler) {
                static_cast<SomeEffect*>(&handler)->UpdateEffectProperties();
            };
        }

        // Override to update the effect properties when the effect state
        // changes
        virtual void UpdateEffectProperties() {}
    };
} // namespace PJ
