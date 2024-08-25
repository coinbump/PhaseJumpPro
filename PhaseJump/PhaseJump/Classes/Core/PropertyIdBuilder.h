#pragma once

#include "List.h"
#include "StringUtils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/17/24
 */
namespace PJ {
    /**
     Builds a property id string. Useful with complex objects that don't change often, but are
     compared often. Instead of comparing every object property, you can compare 2 property id
     strings.

     Example: "name:"Ted";values:[1,2,3,4,]"
     */
    class PropertyIdBuilder {
    protected:
        std::ostringstream ss;
        bool hasValues = false;

        void AddName(String name, String symbol) {
            if (hasValues) {
                ss << ";";
            }
            hasValues = true;

            if (name.size() > 0) {
                ss << name << ":" << symbol;
            } else {
                ss << symbol;
            }
        }

    public:
        PropertyIdBuilder& Add(String name, String value) {
            AddName(name, "");
            ss << '"' << value << '"';
            return *this;
        }

        PropertyIdBuilder& Add(String name, int value) {
            AddName(name, "");
            ss << value;
            return *this;
        }

        PropertyIdBuilder& Add(String name, float value) {
            AddName(name, "");
            ss << value;
            return *this;
        }

        template <class Collection>
        PropertyIdBuilder& AddCollection(String name, Collection const& items) {
            GUARDR(!IsEmpty(items), *this)

            AddName(name, "[");

            for (auto const& item : items) {
                // Leave final trailing comma for simplicity (not all collections have operator[]
                ss << item << ",";
            }

            ss << "]";

            return *this;
        }

        template <class Map>
        PropertyIdBuilder& AddMap(String name, Map const& map) {
            GUARDR(!IsEmpty(map), *this)

            AddName(name, "{");

            for (auto const& keyValue : map) {
                // Leave final trailing comma
                ss << "{" << keyValue.first << "," << keyValue.second << "},";
            }

            ss << "}";

            return *this;
        }

        String Result() const {
            return ss.str();
        }
    };
} // namespace PJ
