#pragma once

#include <algorithm>
#include <functional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/29/24
 */
namespace PJ {
    template <class Input, class Output = Input>
    using TransformFunc = std::function<Output(Input value)>;

    namespace TransformFuncs {
        template <class Input>
        TransformFunc<Input> Identity() {
            return [](Input value) { return value; };
        }

        template <class Input>
        TransformFunc<Input> Clamp(Input minValue, Input maxValue) {
            return [=](Input value) { return std::clamp(value, minValue, maxValue); };
        }

        template <class Input>
        TransformFunc<Input> Multiply(Input factor) {
            return [=](Input value) { return value * factor; };
        }
    } // namespace TransformFuncs
} // namespace PJ
