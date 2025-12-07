#pragma once

#include "Class.h"
#include "OrderedMap.h"
#include "StandardCore.h"
#include "StringUtils.h"
#include "VectorList.h"
#include <optional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// Defines the ids and user-facing display names of an enum for
    /// serialization and editing
    template <typename EnumType>
    class EnumClass : public Class<> {
    private:
        VectorList<EnumType> cases;

    protected:
        /// Maps enum to id used for serialization
        OrderedMap<EnumType, String> toIdMap;

        /// Maps id to enum
        OrderedMap<String, EnumType> toEnumMap;

        /// Maps Enum to name visible to the user
        OrderedMap<EnumType, String> toNameMap;

    public:
        using Base = Class;

        EnumClass(String id) :
            Base(id) {}

        void MapEnum(EnumType value, String id, String name) {
            cases.push_back(value);
            toIdMap[value] = id;
            toEnumMap[id] = value;
            toNameMap[value] = name;
        }

        String Id(EnumType value) const {
            String result;
            auto i = toIdMap.find(value);
            GUARDR(i != toIdMap.end(), result);
            return i->second;
        }

        String Name(EnumType value) const {
            String result;
            auto i = toNameMap.find(value);
            GUARDR(i != toNameMap.end(), result);
            return i->second;
        }

        int ToEnum(String id) const {
            int result = 0;
            auto i = toEnumMap.find(id);
            GUARDR(i != toEnumMap.end(), result);
            return i->second;
        }

        VectorList<EnumType> AllCases() const {
            return cases;
        }

        std::optional<size_t> IndexOf(EnumType value) const {
            auto result = PJ::IndexOf(cases, value);
            return result ? std::make_optional<size_t>(result.value()) : std::optional<size_t>();
        }
    };
} // namespace PJ
