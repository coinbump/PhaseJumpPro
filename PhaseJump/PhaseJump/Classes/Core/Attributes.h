#pragma once

#include "Attribute.h"

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
        UP<Attribute> NameAttribute(String name);

        template <class T>
        UP<Attribute> DefaultValueAttribute(T value) {
            return NEW<ValueAttribute<T>>(AttributeId::DefaultValue, value);
        }

        template <class T>
        UP<Attribute> RangeAttribute(T minValue, T maxValue) {
            return NEW<ValueAttribute<std::pair<T, T>>>(
                AttributeId::DefaultValue, std::pair<T, T>(minValue, maxValue)
            );
        }
    } // namespace Attributes
} // namespace PJ
