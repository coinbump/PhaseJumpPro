#ifndef PJANIMATIONCURVE_H_
#define PJANIMATIONCURVE_H_

#include "FloatTransform.h"
#include "SomeValueInterpolator.h"
#include "SomeInterpolate.h"
#include <memory>

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 1/12/24
 */
namespace PJ
{
    /// <summary>
    /// Specifies the details of an animation curve from start to end values, with an interpolation transform
    /// </summary>
    template <class T>
    class Interpolator
    {
    public:
        T start;
        T end;

        /// Interpolates the animated value from start to end
        SP<SomeValueInterpolator<T>> valueInterpolator;

        /// The animation curve
        SP<FloatTransform> transform = MAKE<InterpolateLinear>();

        Interpolator(T start,
                     T end,
                     SP<SomeValueInterpolator<T>> valueInterpolator = MAKE<ValueInterpolator<T>>(),
                     SP<FloatTransform> transform = MAKE<InterpolateLinear>()) :
        start(start),
        end(end),
        valueInterpolator(valueInterpolator),
        transform(transform) {
        }
        virtual ~Interpolator() {}

        T ValueAt(float progress) {
            if (nullptr == valueInterpolator) {
                return T();
            }

            return valueInterpolator->ValueAt(start, end, transform->Transform(progress));
        }
    };
}

#endif
