#pragma once

#include "Binding.h"
#include "EaseFunc.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/5/24
 */
namespace PJ {
    /// Function that returns a value between [start, end] based on progress
    /// Progress value is usually 0-1.0, but can overflow for elastic animations
    template <class T>
    using InterpolateFunc = std::function<T(float progress)>;

    namespace InterpolateFuncs {
        /**
         @return Returns the standard func for interpolating between start and end values

         If you want to create your own interpolate func Make specializations you can use C++
         template specialization

         Example:
         template <>
         InterpolateFunc<String>Make(String start, String end);
         */
        template <class T>
        InterpolateFunc<T> Make(T start, T end) {
            return [=](float progress) { return start + (end - start) * progress; };
        }

        /// @return Returns an interpolation func that applies an ease curve to the progress
        /// parameter
        template <class T>
        InterpolateFunc<T> Ease(InterpolateFunc<T> func, EaseFunc ease = EaseFuncs::linear) {
            return [=](float progress) {
                GUARDR(func && ease, T())
                return func(ease(progress));
            };
        }

        /// @return Returns an interpolation func that interpolates in reverse (from end to start)
        template <class T>
        InterpolateFunc<T> Reverse(InterpolateFunc<T> func) {
            return [=](float progress) {
                GUARDR(func, T())
                return func(1.0f - progress);
            };
        }

        /// @return Returns a set binding func that takes a progress value, calls the interpolator,
        /// and sets the value via the binding
        template <class T>
        SetBindingFunc<T> Binding(InterpolateFunc<T> func, SetBindingFunc<T> binding) {
            return [=](float progress) {
                GUARD(func && binding)
                binding(func(progress));
            };
        }
    } // namespace InterpolateFuncs
} // namespace PJ
