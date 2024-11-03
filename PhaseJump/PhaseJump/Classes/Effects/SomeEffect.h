#pragma once

#include "Switchable.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 11/2/24
 */
namespace PJ {
    /// A effect is applied when enabled
    /// Used as a type filter when enabling/disabling components (Example: EnableEffectsById)
    class SomeEffect : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = SomeEffect;

        SomeEffect(SwitchState state = SwitchState::Off) {
            Enable(state == SwitchState::On);
        }
    };
} // namespace PJ
