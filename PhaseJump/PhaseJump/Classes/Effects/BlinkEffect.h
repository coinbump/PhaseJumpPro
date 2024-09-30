#pragma once

#include "AnimationCycleTypes.h"
#include "SomeEffect.h"
#include "TimeTrack.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/28/24
 */
namespace PJ {
    class SomeRenderer;

    /// Turns renderers on/off based on a time track
    class BlinkEffect : public SomeEffect {
    public:
        using Base = SomeEffect;
        using This = BlinkEffect;

    protected:
        TimeTrack<bool> timeTrack;

    public:
        BlinkEffect(
            AnimationCycleType cycleType, VectorList<float> offKeys, VectorList<float> onKeys
        );

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "BlinkEffect";
        }
    };
} // namespace PJ
