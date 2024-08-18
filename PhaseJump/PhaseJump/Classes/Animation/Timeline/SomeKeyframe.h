#ifndef PJSOMEKEYFRAME_H
#define PJSOMEKEYFRAME_H

#include "SomeKeyframeInterpolatorFactory.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 5/18/24
 */
namespace PJ {
    /// Keyframe at a specific point in time
    class SomeKeyframe {
    public:
        using MathType = float;

        virtual ~SomeKeyframe() {}

        MathType time = MathType();
    };
} // namespace PJ

#endif
