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
    class SomeAttribute {
    public:
        String id;

        SomeAttribute(String id) :
            id(id) {}
    };

    /// Attribute with a value
    template <class Type = Void>
    class Attribute : public SomeAttribute {
    public:
        using Base = SomeAttribute;

        Type value{};

        Attribute(String id, Type value = Type()) :
            Base(id),
            value(value) {}
    };
} // namespace PJ
