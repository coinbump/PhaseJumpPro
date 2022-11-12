#ifndef PJCLASS_H_
#define PJCLASS_H_

#include <string>
#include "Base.h"
#include "Tags.h"
#include "Factory.h"

/*
 RATING: 5 stars
 Utility class, no functionality
 CODE REVIEW: 11/6/22
 */
namespace PJ {
    /// <summary>
    /// Class objects are useful for defining modular behavior based on object type
    /// We might want to have properties that are defined dynamically (during runtime),
    /// but also shared by multiple objects of the same type
    /// </summary>
    class Class {
    public:
        std::string id;

        /// <summary>
        /// Optional title, for browsing
        /// </summary>
        std::string title;

        /// <summary>
        /// Optional description, for browsing
        /// </summary>
        std::string description;

        /// <summary>
        /// Set of string tags that define type properties
        /// </summary>
        TypeTagsSet typeTags;

        /// <summary>
        /// Tag metadata
        /// </summary>
        Tags tags;

        Class(std::string id) : id(id) {
        }

        virtual ~Class() {}

        virtual PJ::Base* New() { return nullptr; }
    };

    template <class Type> class TypeClass : public Class {
    public:
        typedef Class Base;
        typedef Type* NewType;
        typedef Factory<NewType> Factory;
        typedef std::shared_ptr<Factory> FactorySharedPtr;

        FactorySharedPtr factory;

        TypeClass(std::string id, FactorySharedPtr factory = std::make_shared<Factory>([] () -> NewType { return new Type(); })) : Base(id), factory(factory) {
        }

        PJ::Base* New() override {
            if (!factory.get()) { return NULL; }

            return factory->New();
        }
    };
}

#endif
