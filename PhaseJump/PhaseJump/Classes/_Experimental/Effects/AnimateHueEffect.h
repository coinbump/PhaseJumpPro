#pragma once

#include "ModelColor.h"
#include "SomeEffect.h"
#include "SwitchHandler.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 5/11/23
 */
namespace PJ {
    class SomeRenderer;

    // TODO: re-evaluate all effects
    /// Animates the material's render color across the range of hues
    class AnimateHueEffect : public SomeEffect {
    public:
        using Base = WorldComponent<>;
        using This = AnimateHueEffect;

        AnimateHueEffect() {}

        /// Time for a full cyle across all hues
        float cycleTime = 1;

        // Range(0...1)
        float hue = 1;

        // Range(0...1)
        float saturation = 1;

        // Range(0...1)]
        float value = 1;

    protected:
        SP<SomeRenderer> renderer;

    public:
        void Awake() override;
        void OnUpdate(TimeSlice time) override;
        void UpdateEffectProperties() override;
    };
} // namespace PJ
