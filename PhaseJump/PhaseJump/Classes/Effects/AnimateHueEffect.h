#ifndef PJANIMATEHUEEFFECT_H
#define PJANIMATEHUEEFFECT_H

#include "SomeEffect.h"
#include "RendererTool.h"
#include "ModelColor.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 5/11/23
 */
namespace PJ
{
    /// <summary>
    /// Animates the material's render color across the range of hues
    /// </summary>
    class AnimateHueEffect : public SomeEffect
    {
    public:
        using Base = SomeEffect;

        /// <summary>
        /// Time for a full cyle across all hues
        /// </summary>
        float cycleTime = 1.0f;

        // Range(0, 1.0f)
        float hue = 1.0f;

        // Range(0, 1.0f)
        float saturation = 1.0f;

        // Range(0, 1.0f)]
        float value = 1.0f;

    protected:
        SP<RendererTool> rendererTool;

    public:
        void Awake() override;
        void OnUpdate(TimeSlice time) override;
        void UpdateEffectProperties() override;
    };
}

#endif
