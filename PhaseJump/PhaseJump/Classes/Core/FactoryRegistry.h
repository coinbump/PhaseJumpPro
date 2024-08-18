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
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Store factories accessed by a class ID
    template <class Type, typename... Arguments>
    class FactoryRegistry {
    public:
        using Map = UnorderedMap<String, SP<Factory<Type, Arguments...>>>;

        Map map;

        using ModifierFunc = PJ::ModifierFunc<Type>;

        List<ModifierFunc> modifiers;

        FactoryRegistry() {}

        SP<Type> New(String id, Arguments... args) {
            auto i = map.find(id);
            GUARDR(i != map.end(), SP<Type>());

            auto& factory = i->second;
            auto result = factory->New(args...);

            for (auto& modifier : modifiers) {
                modifier(*result);
            }

            return result;
        }
    };
} // namespace PJ
