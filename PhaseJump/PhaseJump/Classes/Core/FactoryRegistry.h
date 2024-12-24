#pragma once

#include "Factory.h"
#include "List.h"
#include "ModifierFunc.h"
#include "UnorderedMap.h"
#include <map>
#include <string>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Store factories accessed by a class ID
    template <class Type, typename... Arguments>
    class FactoryRegistry {
    public:
        using Map = UnorderedMap<String, UP<Factory<Type, Arguments...>>>;

        Map map;

        FactoryRegistry() {}

        SP<Type> Make(String id, Arguments... args) {
            return std::move(New(id, args...));
        }

        UP<Type> New(String id, Arguments... args) {
            auto i = map.find(id);
            GUARDR(i != map.end(), {});

            auto& factory = i->second;
            auto result = factory->New(args...);

            return result;
        }
    };
} // namespace PJ
