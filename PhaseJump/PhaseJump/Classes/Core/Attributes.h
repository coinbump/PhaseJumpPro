#pragma once

#include "SomeAttribute.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    namespace AttributeId {
        auto constexpr DefaultValue = "default.value";
        auto constexpr Name = "name";
        auto constexpr Range = "range";
    } // namespace AttributeId

    namespace Attributes {
        SP<SomeAttribute> NameAttribute(String name);

        template <class T>
        SP<SomeAttribute> DefaultValueAttribute(T value) {
            return MAKE<Attribute<T>>(AttributeId::DefaultValue, value);
        }

        template <class T>
        SP<SomeAttribute> RangeAttribute(T minValue, T maxValue) {
            return MAKE<Attribute<std::pair<T, T>>>(
                AttributeId::DefaultValue, std::pair<T, T>(minValue, maxValue)
            );
        }
    } // namespace Attributes
} // namespace PJ
