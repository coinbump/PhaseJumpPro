#pragma once

#include "ModelColor.h"
#include "SomeEffect.h"
#include "SwitchHandler.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/7/24
 */
namespace PJ {
    class SomeRenderer;

    /// Animates the material's render color across the range of hues
    class AnimateHueEffect : public SomeEffect {
    public:
        using Base = WorldComponent<>;
        using This = AnimateHueEffect;

        AnimateHueEffect() {}

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
        void Awake() override;
        void OnUpdate(TimeSlice time) override;
        void UpdateEffectProperties() override;
    };
} // namespace PJ
