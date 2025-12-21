#pragma once

#include "Class.h"
#include "Factory.h"
#include "StringUtils.h"
#include "TypeClass.h"
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
    template <class ClassType = Class>
    class ClassRegistry {
    protected:
        using ClassMap = UnorderedMap<String, UP<ClassType>>;

        ClassMap map;

    public:
        ClassRegistry() {}

        ClassMap const& Map() const {
            return map;
        }

        /// Adds a class to the registry
        void Add(UP<ClassType>& _class) {
            Add(std::move(_class));
        }

        /// Adds a class to the registry
        void Add(UP<ClassType>&& _class) {
            GUARD(_class)
            map.insert_or_assign(_class->_core.id, std::move(_class));
        }

        /// Adds a class to the registry and set the id
        void Add(String id, UP<ClassType>&& _class) {
            GUARD(_class)
            _class->_core.id = id;
            Add(_class);
        }

        // TODO: use std::requires
        template <
            class Type, typename... Arguments, typename _Type = ClassType,
            std::enable_if_t<std::is_base_of<TypeClass<Type, Arguments...>, _Type>::value>* =
                nullptr>
        UP<Type> NewType(String key, Arguments... args) {
            auto iterator = map.find(key);
            GUARDR(iterator != map.end(), {});

            auto& ptr = iterator->second;
            return ptr->New(args...);
        }

        /// @return Returns a new object for the class type, if the class is a TypeClass and has a
        /// factory
        template <
            class Type, typename... Arguments, typename _Type = ClassType,
            std::enable_if_t<std::is_base_of<TypeClass<Type, Arguments...>, _Type>::value>* =
                nullptr>
        SP<Type> MakeType(String key, Arguments... args) {
            return std::move(NewType<Type>(key, args...));
        }
    };
} // namespace PJ
