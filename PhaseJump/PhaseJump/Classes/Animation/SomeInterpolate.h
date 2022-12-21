#ifndef PJSOMEINTERPOLATE_H
#define PJSOMEINTERPOLATE_H

#include <math.h>
#include <memory>
#include "FloatTransform.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 11/26/22
 */
namespace PJ
{
    /// <summary>
    /// For editors to pick from list of types
    /// </summary>
    enum class InterpolateType
    {
        Linear,

        InSquared,
        InCubed,
        OutSquared,
        OutCubed,
        OutElastic,
        InBack,
        OutBack,
        InOutBack
    };

    /// <summary>
    /// Interpolate a normalized float.(0-1.0)
    /// Like Unity's AnimationCurve. Platform neutral for future language porting
    /// </summary>
    class SomeInterpolate : public FloatTransform
    {
    };

    class LinearInterpolate : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return time;
        }
    };

    class EaseInSquared : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return pow(time, 2);
        }
    };

    class EaseInCubed : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return pow(time, 3);
        }
    };

    class EaseOutSquared : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return 1.0f - pow(1.0f - time, 2);
        }
    };

    class EaseOutCubed : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return  1.0f - pow(1.0f - time, 3);
        }
    };

    class EaseOutElastic : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c4 = (2.0f * M_PI) / 3.0f;
            return x == 0 ? 0 : x == 1 ? 1 : pow(2, -15 * x) * sin((x * 10.0f - 0.75f) * c4) + 1.0f;
        }
    };

    class EaseInBack : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c1 = 1.70158f;
            float c3 = c1 + 1.0f;
            return c3 * x * x * x - c1 * x * x;
        }
    };

    class EaseOutBack : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c1 = 1.70158f;
            float c3 = c1 + 1.0f;
            return 1.0f + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
        }
    };

    class EaseInOutBack : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c1 = 1.70158f;
            float c2 = c1 * 1.525f;
            return x < 0.5f
            ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0f
            : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0f;
        }
    };

    namespace _InterpolateType {
        extern const std::shared_ptr<SomeInterpolate> linear;
        extern const std::shared_ptr<SomeInterpolate> easeInSquared;
        extern const std::shared_ptr<SomeInterpolate> easeInCubed;
        extern const std::shared_ptr<SomeInterpolate> easeOutSquared;
        extern const std::shared_ptr<SomeInterpolate> easeOutCubed;
        extern const std::shared_ptr<SomeInterpolate> easeOutElastic;
        extern const std::shared_ptr<SomeInterpolate> easeInBack;
        extern const std::shared_ptr<SomeInterpolate> easeOutBack;
        extern const std::shared_ptr<SomeInterpolate> easeInOutBack;
    };
}

#endif
