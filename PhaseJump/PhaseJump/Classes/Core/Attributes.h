#pragma once

#include "SomeAttribute.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    namespace AttributeId {
        auto constexpr DefaultValue = "default.value";
        auto constexpr Name = "name";
        auto constexpr Range = "range";
    } // namespace AttributeId

    namespace Attributes {
        UP<SomeAttribute> NameAttribute(String name);

        template <class T>
        UP<SomeAttribute> DefaultValueAttribute(T value) {
            return NEW<Attribute<T>>(AttributeId::DefaultValue, value);
        }

        template <class T>
        UP<SomeAttribute> RangeAttribute(T minValue, T maxValue) {
            return NEW<Attribute<std::pair<T, T>>>(
                AttributeId::DefaultValue, std::pair<T, T>(minValue, maxValue)
            );
        }
    } // namespace Attributes
} // namespace PJ
