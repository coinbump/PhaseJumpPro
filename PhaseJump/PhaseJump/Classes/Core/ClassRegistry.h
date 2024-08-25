#pragma once

#include "Collections/UnorderedMap.h"
#include "Factory.h"
#include "StringUtils.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Registry for classes
    template <class BaseType = Base>
    class ClassRegistry {
    public:
        using Map = UnorderedMap<String, SP<SomeClass<Base>>>;

        Map map;

        ClassRegistry() {}

        template <class T>
        SP<T> NewType(String key) {
            return DCAST<T>(NewBase(key));
        }

        SP<Base> NewBase(String key) const {
            auto iterator = map.find(key);
            GUARDR(iterator != map.end(), nullptr);

            auto ptr = iterator->second;
            auto value = dynamic_cast<SomeSomeFactoryProvider<Base>*>(ptr.get());
            GUARDR(value, nullptr)

            SomeFactory<Base>* factory;
            value->Provide(factory);
            GUARDR(factory, nullptr)

            auto concreteFactory = As<SomeFactory<Base>>(factory);
            GUARDR(concreteFactory, nullptr)

            return concreteFactory->NewBase();
        }
    };
} // namespace PJ
