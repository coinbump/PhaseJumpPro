#ifndef PJSOMEKEYFRAMEINTERPOLATORFACTORY_H
#define PJSOMEKEYFRAMEINTERPOLATORFACTORY_H

#include "Interpolator.h"
#include <memory>

namespace PJ
{
    /// <summary>
    /// Produces an interpolator based on key values
    /// </summary>
    template <class T>
    class SomeKeyframeInterpolatorFactory {
    public:
        virtual SP<Interpolator<T>> NewInterpolator(T start, T end, SomeKeyframeInterpolatorFactory<T> nextKey) = 0;
    };

    /// <summary>
    /// Allows us to set an interpolation curve from one keyframe to the next
    /// FUTURE: support Unity-style tangent/weight type animation curves
    /// </summary>
    class KeyframeInterpolatorFactory : public SomeKeyframeInterpolatorFactory<float> {
    public:
        SP<SomeInterpolate> interpolate;

        SP<Interpolator<float>> NewInterpolator(float start, float end, SomeKeyframeInterpolatorFactory<float> nextKey) override
        {
            return MAKE<Interpolator<float>>(start, end, MAKE<ValueInterpolator<float>>(), interpolate);
        }
    };

    // FUTURE: support SplineAnimationCurve
}

#endif
