#ifndef PJANIMATIONCURVE_H_
#define PJANIMATIONCURVE_H_

#include "Types/FloatTransform.h"
#include "SomeValueInterpolator.h"
#include "SomeInterpolate.h"
#include <memory>

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 12/17/22
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
        std::shared_ptr<SomeValueInterpolator<T>> valueInterpolator;
        std::shared_ptr<FloatTransform> transform = std::make_shared<LinearInterpolate>();

        Interpolator(T start,
                     T end,
                     std::shared_ptr<SomeValueInterpolator<T>> valueInterpolator = std::make_shared<ValueInterpolator<T>>(),
                     std::shared_ptr<FloatTransform> transform = std::make_shared<LinearInterpolate>()) :
        start(start),
        end(end),
        valueInterpolator(valueInterpolator),
        transform(transform) {
        }
        virtual ~Interpolator() {}

        T ValueAt(float progress) {
            return valueInterpolator->ValueAt(start, end, transform->Transform(progress));
        }
    };
}

#endif
