#pragma once

#include "Base.h"
#include "StringUtils.h"
#include "Tags.h"
#include "Utils.h"
#include "Void.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/19/25
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
    class Class {
    public:
        /// Standard core for a class object
        struct StandardCore {
            String id;

            /// Name for browsing
            String name;

            /// Object type attributes
            TypeTagSet typeTags;

            /// Custom properties
            Tags tags;
        };

        StandardCore _core;

        Class(StandardCore const& _core) :
            _core(_core) {}

        virtual ~Class() {}

        virtual String Name() const {
            return _core.name;
        }
    };

    /**
     Defines modular properties based on object type.
     Allows us to set properties for all objects of the same type via composition of the class.
     Example: 3 cards share the same "JokerCardClass" object
     */
    template <class Core = Void>
    class CoreClass : public Class {
    public:
        using Base = Class;

        Core core{};

        CoreClass(StandardCore const& _core, Core core = {}) :
            Class(_core),
            core(core) {}

        virtual ~CoreClass() {}
    };
} // namespace PJ
