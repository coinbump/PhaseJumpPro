#pragma once

#include "Class.h"
#include "Factory.h"
#include "StringUtils.h"
#include "UnorderedMap.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/21/24
 */
namespace PJ {
    /// Registry for classes
    /// Each registered class has metadata like tags, name, etc.
    /// Eacn registered can be a factory that creates a specific type
    template <class BaseType = Base, class ClassType = SomeClass<Base>>
    class ClassRegistry {
    public:
        using Map = UnorderedMap<String, UP<ClassType>>;

        Map map;

        ClassRegistry() {}

        void Add(UP<ClassType>& _class) {
            Add(std::move(_class));
        }

        void Add(UP<ClassType>&& _class) {
            GUARD(_class)
            map.insert_or_assign(_class->id, std::move(_class));
        }

        template <class T>
        SP<T> NewType(String key) {
            return DCAST<T>(NewBase(key));
        }

        SP<Base> NewBase(String key) const {
            auto iterator = map.find(key);
            GUARDR(iterator != map.end(), {});

            auto& ptr = iterator->second;
            auto factory = dynamic_cast<SomeFactory<Base>*>(ptr.get());
            GUARDR(factory, {})

            return factory->NewBase();
        }
    };
} // namespace PJ
