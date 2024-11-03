#pragma once

#include "List.h"
#include "StringUtils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 10/22/24
 */
namespace PJ {
    /**
     Builds a property id string that is also valid JSON
     Used to map or compare complex objects using a single string

     Example: {"keyCode":122,"modifiers":["shortcut"]}
     */
    class PropertyIdBuilder {
    protected:
        enum class QuoteType {
            Default,

            Quote,

            None
        };

        std::ostringstream ss;
        bool hasValues = false;

        void AddName(String name, String symbol) {
            if (hasValues) {
                ss << ",";
            }
            hasValues = true;

            ss << "\"" << name << "\"" << symbol;
        }

    public:
        template <class Type>
        PropertyIdBuilder& AddValue(Type value) {
            ss << value;
            return *this;
        }

        template <>
        PropertyIdBuilder& AddValue<String>(String value);

        template <>
        PropertyIdBuilder& AddValue<const char*>(const char* value);

        template <class Type>
        PropertyIdBuilder& Add(String name, Type value) {
            AddName(name, ":");
            AddValue(value);
            return *this;
        }

        template <class Collection>
        PropertyIdBuilder& AddCollection(String name, Collection const& _items) {
            VectorList<typename Collection::value_type> items(_items.begin(), _items.end());
            GUARDR(!IsEmpty(items), *this)

            AddName(name, ":[");

            for (size_t i = 0; i < items.size(); i++) {
                auto& item = items[i];
                AddValue(item);
                if (i < items.size() - 1) {
                    ss << ",";
                }
            }

            ss << "]";

            return *this;
        }

        template <class Map>
        PropertyIdBuilder& AddMap(String name, Map const& map) {
            GUARDR(!IsEmpty(map), *this)

            PropertyIdBuilder inner;
            inner.AddMapInner(name, map);
            AddName(name, ":" + inner.Result());

            return *this;
        }

        String Result() const {
            std::stringstream ss;

            ss << "{";
            ss << this->ss.str();
            ss << "}";

            return ss.str();
        }

    protected:
        template <class Map>
        PropertyIdBuilder& AddMapInner(String name, Map const& map) {
            GUARDR(!IsEmpty(map), *this)

            VectorList<typename Map::key_type> keys;

            for (auto& keyValue : map) {
                keys.push_back(keyValue.first);
            }

            for (size_t i = 0; i < keys.size(); i++) {
                auto key = keys[i];

                try {
                    Add(key, map.at(key));
                } catch (...) {}
            }

            return *this;
        }
    };
} // namespace PJ
