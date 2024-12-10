#pragma once

#include "AnimationCycleTypes.h"
#include "Switchable.h"
#include "TimeTrack.h"
#include "VectorList.h"
#include "WorldComponent.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/28/24
 */
namespace PJ {
    class SomeRenderer;

    /// Turns renderers on/off based on a time track
    class BlinkEffect : public WorldComponent<> {
    public:
        using Base = WorldComponent<>;
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
