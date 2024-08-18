#ifndef PJANIMATEHUEEFFECT_H
#define PJANIMATEHUEEFFECT_H

#include "ModelColor.h"
#include "RendererTool.h"
#include "SomeEffect.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 5/11/23
 */
namespace PJ {
    // TODO: re-evaluate all effects
    /// Animates the material's render color across the range of hues
    class AnimateHueEffect : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
        using This = AnimateHueEffect;

        AnimateHueEffect() {
            // TODO: switchHandler = MAKE<ComposeSwitchHandler>(ComposeSwitchHandler(*this));
        }

        /// Time for a full cyle across all hues
        float cycleTime = 1;

        // Range(0...1)
        float hue = 1;

        // Range(0...1)
        float saturation = 1;

        // Range(0...1)]
        float value = 1;

    protected:
        SP<RendererTool> rendererTool;

    public:
        void Awake() override;
        void OnUpdate(TimeSlice time) override;
        void UpdateEffectProperties();

        void OnSwitchChange() {
            UpdateEffectProperties();
        }
    };
} // namespace PJ

#endif
