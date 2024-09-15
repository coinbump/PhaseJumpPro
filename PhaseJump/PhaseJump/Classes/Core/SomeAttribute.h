#pragma once

#include "StringUtils.h"
#include "Void.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// Defines additional custom attributes for an object
    class SomeAttribute {
    public:
        String id;

        SomeAttribute(String id) :
            id(id) {}
    };

    template <class Core = Void>
    class Attribute : public SomeAttribute {
    public:
        using Base = SomeAttribute;

        Core core{};

        Attribute(String id, Core core = Core()) :
            Base(id),
            core(core) {}
    };
} // namespace PJ
