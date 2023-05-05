#ifndef PJENUMCLASS_H
#define PJENUMCLASS_H

#include "_String.h"
#include "_Map.h"
#include "Core.h"
#include "Class.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/30/23
 */
namespace PJ {
    /// Defines the ids and user-facing display names of an enum for serialization and editing
    template <typename EnumType>
    class EnumClass : public Class
    {
    private:
        VectorList<EnumType> cases;

    protected:
        /// Maps enum to name used for serialization
        Map<EnumType, String> toIdMap;

        /// Maps id to enum
        Map<String, EnumType> toEnumMap;

        /// Maps Enum to name visible to the user
        Map<EnumType, String> toNameMap;

    public:
        using Base = Class;

        EnumClass(String id) : Base(id) {
        }

        void MapEnum(EnumType value, String id, String name) {
            cases.Add(value);
            toIdMap[value] = id;
            toEnumMap[id] = value;
            toNameMap[value] = name;
        }

        String IdOf(EnumType value) {
            return toIdMap[value];
        }

        String NameOf(EnumType value) {
            return toNameMap[value];
        }

        int ToEnum(String id) const {
            return toEnumMap[id];
        }

        VectorList<EnumType> AllCases() const { return cases; }
        size_t IndexOf(EnumType value) const {
            auto result = cases.IndexOf(value);
            return result ? result.value() : 0;
        }
    };
}

#endif
