#pragma once

#include "Base.h"
#include "StringUtils.h"
#include "Tags.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/12/24
 */
namespace PJ {
    /// Interface for object that defines properties shared based on object type
    template <class BaseType = Base>
    class SomeClass {
    public:
        virtual ~SomeClass() {}
    };

    /// Standard core for a class object
    /// Or you can define your own
    struct StandardClassCore {
        /// Name, for browsing
        String name;

        /// Description, for browsing
        String description;

        /// Object attribute types (what kind of object is this?)
        TypeTagSet typeTags;

        /// Custom properties
        Tags tags;
    };

    /// Defines modular properties based on object type
    /// Allows us to set properties for all objects of the same type via
    /// composition of the class Example: 3 cards share the same
    /// "JokerCardClass" object
    template <class Core = StandardClassCore, class BaseType = Base>
    class Class : public OwnerBase<Core>, public SomeClass<BaseType> {
    public:
        String id;

        Class(String id) :
            id(id) {}

        virtual ~Class() {}
    };
} // namespace PJ
