#pragma once

#include "StringUtils.h"
#include "Void.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// Defines additional custom attributes for an object
    class Attribute {
    public:
        String id;

        Attribute(String id) :
            id(id) {}
    };

    /// Attribute with a value
    template <class Type = Void>
    class ValueAttribute : public Attribute {
    public:
        using Base = Attribute;

        Type value{};

        ValueAttribute(String id, Type value = Type()) :
            Base(id),
            value(value) {}
    };
} // namespace PJ
