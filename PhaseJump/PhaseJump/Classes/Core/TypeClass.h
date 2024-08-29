#pragma once

#include "Class.h"
#include "Factory.h"
#include "FactoryRegistry.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/12/24
 */
namespace PJ {
    /// Provides a default factory object to create objects of the related type
    template <class Type, class Core = StandardClassCore>
    class TypeClass : public Class<Core, typename Type::RootBaseType>,
                      public SomeSomeFactoryProvider<typename Type::RootBaseType> {
    public:
        /// Type must define a root base type
        using BaseType = Type::RootBaseType;
        using Base = Class<Core, BaseType>;
        using NewType = SP<Type>;
        using Factory = Factory<Type>;
        using FactoryFunc = std::function<SP<Type>()>;

        /// Factory to produce an object of this class type
        UP<Factory> factory;

        TypeClass(String id, FactoryFunc allocator) :
            Base(id),
            factory(std::make_unique<Factory>(allocator)) {}

        SP<Type> Make() {
            GUARDR(factory, nullptr)
            return factory->New();
        }

        // MARK: SomeSomeFactoryProvider

        void Provide(SomeFactory<BaseType>*& result) override {
            result = factory.get();
        }
    };
} // namespace PJ
