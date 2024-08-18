#pragma once

#include "List.h"
#include "SomeAttribute.h"
#include "TagClassId.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// Class properties for a specific tag
    /// Used to build objects and UI from attribute information such as name, default value, and
    /// range attributes
    class SomeTagClass : public Class<> {
    public:
        using Base = Class<>;

        String id;

        List<SP<SomeAttribute>> attributes;

        virtual String Type() const = 0;

        SomeTagClass(String id) :
            Base(id) {}

        SomeTagClass& Add(SP<SomeAttribute> attribute) {
            attributes.push_back(attribute);
            return *this;
        }
    };

    template <class T>
    class TagClass : public SomeTagClass {};

    template <>
    class TagClass<float> : public SomeTagClass {
    public:
        TagClass(String id) :
            SomeTagClass(id) {}

        String Type() const override {
            return TagClassId::Float;
        }
    };

    template <>
    class TagClass<int> : public SomeTagClass {
    public:
        TagClass(String id) :
            SomeTagClass(id) {}

        String Type() const override {
            return TagClassId::Int;
        }
    };

    template <>
    class TagClass<String> : public SomeTagClass {
    public:
        TagClass(String id) :
            SomeTagClass(id) {}

        String Type() const override {
            return TagClassId::String;
        }
    };
} // namespace PJ
