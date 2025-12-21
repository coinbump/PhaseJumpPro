#pragma once

#include "Attribute.h"
#include "List.h"
#include "TagClassId.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// Class properties for a specific tag
    /// Used to build objects and UI from attribute information such as name, default value, and
    /// range attributes
    class TagClass : public Class {
    public:
        using Base = Class;
        using This = TagClass;

        VectorList<UP<Attribute>> attributes;

        virtual String Type() const = 0;

        TagClass(String id) :
            Base({ .id = id }) {}

        TagClass& Add(UP<Attribute>& attribute) {
            attributes.push_back(std::move(attribute));
            return *this;
        }

        TagClass& Add(UP<Attribute>&& attribute) {
            attributes.push_back(std::move(attribute));
            return *this;
        }
    };

    template <class T>
    class TypeTagClass : public TagClass {};

    template <>
    class TypeTagClass<float> : public TagClass {
    public:
        TypeTagClass(String id) :
            TagClass(id) {}

        // MARK: TagClass

        String Type() const override {
            return TagClassId::Float;
        }
    };

    template <>
    class TypeTagClass<int> : public TagClass {
    public:
        TypeTagClass(String id) :
            TagClass(id) {}

        // MARK: TagClass

        String Type() const override {
            return TagClassId::Int;
        }
    };

    template <>
    class TypeTagClass<String> : public TagClass {
    public:
        TypeTagClass(String id) :
            TagClass(id) {}

        // MARK: TagClass

        String Type() const override {
            return TagClassId::String;
        }
    };
} // namespace PJ
