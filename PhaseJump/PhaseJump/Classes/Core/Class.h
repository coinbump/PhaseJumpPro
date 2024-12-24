#pragma once

#include "Base.h"
#include "StringUtils.h"
#include "Tags.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /**
     Interface for object that defines properties shared based on object type

     This is useful when we want to define "meta properties". Example: all characters have a default
     animation. By storing these meta properties in the class object, any object for the class can
     get access to shared properties.

     In addition, TypeClass stores a factory which can be used to create objects of the
     corresponding type.
     */
    class SomeClass {
    public:
        String id;

        SomeClass(String id) :
            id(id) {}

        virtual ~SomeClass() {}
    };

    /// Standard core for a class object
    struct StandardClassCore {
        /// Name for browsing
        String name;

        /// Description for browsing
        String description;

        /// Object type attributes
        TypeTagSet typeTags;

        /// Custom properties
        Tags tags;
    };

    /**
     Defines modular properties based on object type.
     Allows us to set properties for all objects of the same type via composition of the class.
     Example: 3 cards share the same "JokerCardClass" object
     */
    template <class Core = StandardClassCore>
    class Class : public SomeClass {
    public:
        Core core{};

        Class(String id, Core core = {}) :
            SomeClass(id),
            core(core) {}

        virtual ~Class() {}
    };
} // namespace PJ
