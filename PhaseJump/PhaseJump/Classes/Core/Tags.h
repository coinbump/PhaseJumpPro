#pragma once

#include "StringConvertible.h"
#include "StringUtils.h"
#include "UnorderedMap.h"
#include "UnorderedSet.h"
#include <any>
#include <optional>
#include <utility>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    using Tag = std::any;

    /// Allocator struct uses the default constructor to return a value
    template <class T>
    struct DefaultAllocator {
        T operator()() {
            return {};
        }
    };

    /// Holds custom attributes (tags) in a map
    class Tags : public StringConvertible {
    public:
        using Key = String;
        using MapValue = Tag;
        using Map = UnorderedMap<Key, MapValue>;

    protected:
        Map map;

    public:
        virtual ~Tags() {}

        /// Removes the value for this key from the tags map
        void Remove(Key key) {
            map.erase(key);
        }

        std::pair<Map::iterator, bool> Insert(Map::value_type&& __x) {
            return map.insert_or_assign(__x.first, __x.second);
        }

        std::pair<Map::iterator, bool> Add(Map::value_type&& __x) {
            return map.insert_or_assign(__x.first, __x.second);
        }

        /// Sets the value for this key in the tags map
        std::pair<Map::iterator, bool> Set(Key const key, MapValue value) {
            return Insert({ key, value });
        }

        /// Sets this value if the value argument exists, or removes the existing value if value
        /// argument does not exist
        template <class Type>
        void SetOrRemove(Key const key, std::optional<Type> value) {
            if (value) {
                Set(key, *value);
            } else {
                Remove(key);
            }
        }

        /// Returns an optional with the value if it exists, or an empty optional if it doesn't
        /// exist
        template <class T>
        std::optional<T> Value(Key key) const {
            try {
                MapValue const& mapValue = map.at(key);
                auto castValue = std::any_cast<T>(mapValue);
                return castValue;
            } catch (...) {}

            return std::nullopt;
        }

        /// Returns a default value if the requested value doesn't exist
        template <class T>
        T SafeValue(Key key, std::function<T()> allocator) const {
            try {
                MapValue const& mapValue = map.at(key);
                auto castValue = std::any_cast<T>(mapValue);
                return castValue;
            } catch (...) {}

            return allocator();
        }

        /// Returns a default value if the requested value doesn't exist
        template <class Type>
        Type SafeValue(Key key, Type defaultValue = {}) const {
            return SafeValue<Type>(key, [&]() { return defaultValue; });
        }

        /// Returns the value of the specified type, with the specified key
        /// Throws an exception of the value doesn't exist or is the wrong type
        template <class T>
        T At(Key key) const {
            MapValue const& value = map.at(key);
            return std::any_cast<T>(value);
        }

        /// Adds the type value if a tag of this type and id do not exist
        template <class T>
        void TypeAddIfNeeded(Key const key, T value = {}) {
            GUARD(!TypeContains<T>(key))
            Set(key, value);
        }

        /// @return Returns true if the value exists, and is of the correct type
        template <class T>
        bool TypeContains(Key key) const {
            try {
                MapValue const& value = map.at(key);
                std::any_cast<T>(value);
                return true;
            } catch (...) {
                return false;
            }
        }

        size_t size() const {
            return map.size();
        }

        size_t Count() const {
            return size();
        }

        Tags operator+(Tags const& rhs) const {
            Tags result = *this;
            result += rhs;
            return result;
        }

        Tags& operator+=(Tags const& rhs) {
            map.insert(rhs.map.begin(), rhs.map.end());
            return *this;
        }

        // MARK: - StringConvertible

        String ToString() const override {
            std::ostringstream ss;

            for (auto& tagI : map) {
                ss << tagI.first << ": ";

                try {
                    ss << std::any_cast<int>(tagI.second) << std::endl;
                    continue;
                } catch (...) {}

                try {
                    ss << std::any_cast<String>(tagI.second) << std::endl;
                    continue;
                } catch (...) {}

                try {
                    ss << std::any_cast<float>(tagI.second) << std::endl;
                    continue;
                } catch (...) {}

                ss << std::endl;
            }

            return ss.str();
        }
    };

    /// A set of type tag strings
    /// Example: "enemy", "ghost" tags for an arcade game
    using TypeTagSet = UnorderedSet<String>;

    /// Aggregates tags separately for multiple objects
    template <class Key = String>
    class Storage {
    public:
        UnorderedMap<Key, Tags> map;

        /// Sets a tag value
        template <class Type>
        void Set(Key key, String tagName, Type value) {
            map[key].Set(tagName, value);
        }

        /// @return Returns a tag value, or nullopt if the tag doesn't exist
        template <class Type>
        std::optional<Type> Value(Key key, String tagName, Type defaultValue = {}) const {
            try {
                auto& tags = map.at(key);
                return tags.template Value<Type>(tagName);
            } catch (...) {
                return defaultValue;
            }
        }

        /// @return Returns a tag value, or a default if the tag doesn't exist
        template <class Type>
        Type SafeValue(Key key, String tagName, Type defaultValue = {}) const {
            try {
                auto& tags = map.at(key);
                return tags.template SafeValue<Type>(tagName, defaultValue);
            } catch (...) {
                return defaultValue;
            }
        }

        /// @return Returns value if it exists
        /// @throws Throws error if value doesn't exist
        template <class Type>
        Type At(Key key, String tagName) const {
            auto i = map.find(key);
            GUARD_THROW(i != map.end(), std::invalid_argument("Invalid element id"))

            return i->second.template At<Type>(tagName);
        }
    };
} // namespace PJ
