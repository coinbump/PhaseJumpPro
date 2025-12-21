#pragma once

#include "ModelColor.h"
#include "Switchable.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/7/24
 */
namespace PJ {
    class Renderer;

    /// Animates the material's render color across the range of hues
    class CycleHueEffect : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = CycleHueEffect;

        CycleHueEffect(SwitchState state = SwitchState::Off, float cycleTime = 1) :
            cycleTime(cycleTime) {
            Enable(state == SwitchState::On);
        }

        /// Time for a full cyle across all hues
        float cycleTime = 1;

        /// Normalized HSV hue (0-1)
        float hue = 1;

        /// Normalized HSV saturation (0-1)
        float saturation = 1;

        /// Normalized HSV value (0-1)
        float value = 1;

    protected:
        void UpdateEffect();

        // MARK: SomeWorldComponent

        void Awake() override;
        FinishType OnUpdate(TimeSlice time) override;
    };
} // namespace PJ
