#pragma once

#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/13/24
 PORTED TO: C++, C#
 */
namespace PJ {
    template <class Value, class Result = Value>
    using TransformFunc = std::function<Result(Value value)>;

    namespace TransformFuncs {
        template <class Value>
        TransformFunc<Value> Identity() {
            return [](Value value) { return value; };
        }

        template <class Value>
        TransformFunc<Value> Clamp(Value minValue, Value maxValue) {
            return [=](Value value) { return std::clamp(value, minValue, maxValue); };
        }

        template <class Value>
        TransformFunc<Value> Multiply(Value factor) {
            return [=](Value value) { return value * factor; };
        }
    } // namespace TransformFuncs
} // namespace PJ
