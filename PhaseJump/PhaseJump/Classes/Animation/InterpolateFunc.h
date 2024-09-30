#pragma once

#include "Binding.h"
#include "EaseFunc.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// Function that returns a value between [start, end] based on progress
    /// Progress value is usually 0-1.0, but can overflow for elastic animations
    template <class T>
    using InterpolateFunc = std::function<T(float progress)>;

    namespace InterpolateFuncs {
        template <class T>
        InterpolateFunc<T> Make(T start, T end) {
            return [=](float progress) { return start + (end - start) * progress; };
        }

        // Example of how to create specialization for custom properties
        // template <>
        // InterpolateFunc<String>Make(String start, String end);

        /// Aggregates ease func along with value interpolator
        template <class T>
        InterpolateFunc<T> MakeEase(InterpolateFunc<T> func, EaseFunc ease = EaseFuncs::linear) {
            return [=](float progress) {
                GUARDR(func && ease, T())
                return func(ease(progress));
            };
        }

        template <class T>
        InterpolateFunc<T> MakeReverse(InterpolateFunc<T> func) {
            return [=](float progress) {
                GUARDR(func, T())
                return func(1.0f - progress);
            };
        }

        template <class T>
        SetBindingFunc<T> MakeBinding(InterpolateFunc<T> func, SetBindingFunc<T> binding) {
            return [=](float progress) {
                GUARD(func && binding)
                binding(func(progress));
            };
        }

        //        template <class Type>
        //        using MakeInterpolateWithEaseFunc = std::function<InterpolateFunc<Type>(Type
        //        start, Type end, EaseFunc)>;
        //
        //        /// Generates an interpolate maker func of this type
        //        /// Allows for interpolated types that can't use InterpolateFuncs::Make (Example:
        //        text animation) template <class Type> static MakeInterpolateWithEaseFunc<Type>
        //        InterpolateWithEaseMaker() {
        //            return [=](Type start, Type end, EaseFunc easeFunc) {
        //                return InterpolateFuncs::MakeEase(InterpolateFuncs::Make(start, end),
        //                easeFunc);
        //            };
        //        }
    } // namespace InterpolateFuncs
} // namespace PJ
