#ifndef PJSOMEINTERPOLATE_H
#define PJSOMEINTERPOLATE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "Macros.h"
#include "FloatTransform.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 1/12/24
 */
namespace PJ
{
    /// <summary>
    /// For editors to pick from list of types
    /// </summary>
    enum struct InterpolateType
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

    struct InterpolateLinear : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return time;
        }
    };

    struct InterpolateEaseInSquared : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return powf(time, 2);
        }
    };

    struct InterpolateEaseInCubed : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return powf(time, 3);
        }
    };

    struct InterpolateEaseOutSquared : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return 1.0f - powf(1.0f - time, 2);
        }
    };

    struct InterpolateEaseOutCubed : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            return  1.0f - powf(1.0f - time, 3);
        }
    };

    struct InterpolateEaseOutElastic : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c4 = (2.0f * (float)M_PI) / 3.0f;
            return x == 0 ? 0 : x == 1 ? 1 : powf(2, -15 * x) * sin((x * 10.0f - 0.75f) * c4) + 1.0f;
        }
    };

    struct InterpolateEaseInBack : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c1 = 1.70158f;
            float c3 = c1 + 1.0f;
            return c3 * x * x * x - c1 * x * x;
        }
    };

    struct InterpolateEaseOutBack : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c1 = 1.70158f;
            float c3 = c1 + 1.0f;
            return 1.0f + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
        }
    };

    struct InterpolateEaseInOutBack : public SomeInterpolate
    {
        float Transform(float time) const override
        {
            float x = time;
            float c1 = 1.70158f;
            float c2 = c1 * 1.525f;
            return x < 0.5f
            ? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0f
            : (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0f;
        }
    };

    namespace InterpolateTypes {
        extern const SP<SomeInterpolate> linear;
        extern const SP<SomeInterpolate> easeInSquared;
        extern const SP<SomeInterpolate> easeInCubed;
        extern const SP<SomeInterpolate> easeOutSquared;
        extern const SP<SomeInterpolate> easeOutCubed;
        extern const SP<SomeInterpolate> easeOutElastic;
        extern const SP<SomeInterpolate> easeInBack;
        extern const SP<SomeInterpolate> easeOutBack;
        extern const SP<SomeInterpolate> easeInOutBack;
    };
}

#endif
