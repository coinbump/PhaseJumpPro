#pragma once

#include "Class.h"
#include "Factory.h"
#include "FactoryRegistry.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 10/21/24
 */
namespace PJ {
    /// Provides a default factory object to create objects of the related type
    template <class Type, class Core = StandardClassCore, typename... Arguments>
    class TypeClass : public Class<Core, typename Type::RootBaseType>,
                      public SomeFactory<typename Type::RootBaseType> {
    public:
        /// Type must define a root base type
        using BaseType = Type::RootBaseType;
        using Base = Class<Core, BaseType>;
        using FactoryFunc = std::function<SP<Type>(Arguments... args)>;

        /// Factory to produce an object of this class type
        FactoryFunc factoryFunc;

        TypeClass(String id, FactoryFunc factoryFunc) :
            Base(id),
            factoryFunc(factoryFunc) {}

        SP<Type> Make(Arguments... args) {
            GUARDR(factoryFunc, nullptr)
            return factoryFunc(args...);
        }

        // MARK: SomeSomeFactoryProvider

        SP<BaseType> NewBase(Arguments... args) override {
            GUARDR(factoryFunc, {})
            return factoryFunc(args...);
        }
    };
} // namespace PJ
