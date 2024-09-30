#pragma once

#include "ModelColor.h"
#include "SomeEffect.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/7/24
 */
namespace PJ {
    class SomeRenderer;

    /// Animates the material's render color across the range of hues
    class CycleHueEffect : public SomeEffect {
    public:
        using Base = SomeEffect;
        using This = CycleHueEffect;

        CycleHueEffect(SwitchState state = SwitchState::Off, float cycleTime = 1) :
            Base(state),
            cycleTime(cycleTime) {}

        /// Time for a full cyle across all hues
        float cycleTime = 1;

        /// Normalized HSV hue (0-1)
        float hue = 1;

        /// Normalized HSV saturation (0-1)
        float saturation = 1;

        /// Normalized HSV value (0-1)
        float value = 1;

        /// The target renderer to animate (defaults to first sibling renderer)
        WP<SomeRenderer> renderer;

    public:
        // MARK: SomeWorldComponent

        void Awake() override;

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override;

        // MARK: SomeEffect

        void UpdateEffectProperties() override;
    };
} // namespace PJ
