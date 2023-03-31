#ifndef PJCLASS_H_
#define PJCLASS_H_

#include "_String.h"
#include "Base.h"
#include "Tags.h"
#include "Factory.h"
#include "FactoryRegistry.h"

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
    class Class : public Base {
    public:
        String id;

        /// <summary>
        /// Optional title, for browsing
        /// </summary>
        String title;

        /// <summary>
        /// Optional description, for browsing
        /// </summary>
        String description;

        /// <summary>
        /// Set of string tags that define type properties
        /// </summary>
        TypeTagsSet typeTags;

        /// <summary>
        /// Tag metadata
        /// </summary>
        Tags tags;

        Class(String id) : id(id) {
        }

        virtual ~Class() {}

        virtual std::shared_ptr<PJ::Base> New() { return nullptr; }
    };

    template <class Type>
    class TypeClass : public Class {
    public:
        using FactoryRegistry = PJ::FactoryRegistry<Type>;

        /// Factory registry to produce objects that belong to this class by their class ID
        static FactoryRegistry registry;

        using Base = Class;
        using NewType = std::shared_ptr<Type>;
        using Factory = Factory<Type>;
        using FactorySharedPtr = std::shared_ptr<Factory>;

        /// Factory to produce an object of this class type
        FactorySharedPtr factory;

        TypeClass(String id, FactorySharedPtr factory = std::make_shared<Factory>([] () -> NewType { return std::make_shared<Type>(); })) : Base(id), factory(factory) {
        }

        std::shared_ptr<PJ::Base> New() override {
            if (!factory.get()) { return nullptr; }

            return factory->New();
        }

        std::shared_ptr<Type> New(String classId) {
            return registry.New(classId);
        }
    };
}

#endif
