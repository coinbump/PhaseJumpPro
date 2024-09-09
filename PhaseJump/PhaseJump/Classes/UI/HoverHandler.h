#pragma once

#include "SomeEffect.h"
#include "SomeHoverGestureHandler.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Direct port
 CODE REVIEW: 6/18/23
 */
namespace PJ {
    /// Applies an effect when the pointer is over the object
    class HoverHandler : public SomeHoverGestureHandler {
    public:
        SP<SomeEffect> hoverEffect;

        VectorList<SP<SomeEffect>> moreHoverEffects;

        VectorList<SP<SomeEffect>> HoverEffects() const {
            auto result = moreHoverEffects;
            if (hoverEffect) {
                Add(result, hoverEffect);
            }
            return result;
        }

        void SetIsHovering(bool value) override {
            for (auto& hoverEffect : HoverEffects()) {
                hoverEffect->SetIsOn(value);
            }
        }
    };
} // namespace PJ
