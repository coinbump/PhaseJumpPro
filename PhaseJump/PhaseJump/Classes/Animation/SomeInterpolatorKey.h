#ifndef PJSOMEINTERPOLATORKEY_H
#define PJSOMEINTERPOLATORKEY_H

#include "Interpolator.h"
#include <memory>

namespace PJ
{
    /// <summary>
    /// Produces an interpolator based on key values
    /// </summary>
    template <class T>
    class SomeInterpolatorKey {
    public:
        virtual std::shared_ptr<Interpolator<T>> NewInterpolator(T start, T end, SomeInterpolatorKey<T> nextKey) = 0;
    };

    /// <summary>
    /// Allows us to set an interpolation curve from one keyframe to the next
    /// FUTURE: support Unity-style tangent/weight type animation curves
    /// </summary>
    class InterpolateInterpolatorKey : public SomeInterpolatorKey<float> {
    public:
        std::shared_ptr<SomeInterpolate> interpolate;

        std::shared_ptr<Interpolator<float>> NewInterpolator(float start, float end, SomeInterpolatorKey<float> nextKey) override
        {
            return std::make_shared<Interpolator<float>>(start, end, std::make_shared<ValueInterpolator<float>>(), interpolate);
        }
    };

    // FUTURE: support SplineAnimationCurve
}

#endif
