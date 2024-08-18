#pragma once

#include "EaseFunc.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/15/24
 */
namespace PJ {
    /// Function that interpolates a value from start to end
    /// Progress value is usually 0-1.0, but can overflow for elastic animations
    /// Used so we can introduce interpolation for complex values if needed
    template <class T>
    using InterpolateFunc = std::function<T(float progress)>;

    namespace Interpolator {
        template <class T>
        InterpolateFunc<T> MakeFunc(T start, T end) {
            return [=](float progress) { return start + (end - start) * progress; };
        }

        /// Aggregates ease func along with value interpolator
        template <class T>
        InterpolateFunc<T>
        MakeEaseFunc(InterpolateFunc<T> valueTransform, EaseFunc ease = EaseFuncs::linear) {
            return [=](float progress) {
                GUARDR(valueTransform && ease, T())
                return valueTransform(ease(progress));
            };
        }
    } // namespace Interpolator
} // namespace PJ
