#pragma once

#include "StringConvertible.h"
#include "StringUtils.h"
#include "UnorderedMap.h"
#include "UnorderedSet.h"
#include <any>
#include <optional>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/29/24
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
    struct Tags : public StringConvertible {
        using Key = String;
        using MapValue = Tag;
        using Map = UnorderedMap<Key, MapValue>;

        Map map;

        virtual ~Tags() {}

        std::pair<Map::iterator, bool> Insert(Map::value_type&& __x) {
            return map.insert_or_assign(__x.first, __x.second);
        }

        std::pair<Map::iterator, bool> Add(Map::value_type&& __x) {
            return map.insert_or_assign(__x.first, __x.second);
        }

        std::pair<Map::iterator, bool> Insert(Key const key, MapValue value) {
            return map.insert_or_assign(key, value);
        }

        std::pair<Map::iterator, bool> Add(Key const key, MapValue value) {
            return Insert({ key, value });
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
        template <class T, class Allocator = DefaultAllocator<T>>
        T SafeValue(Key key, Allocator allocator = Allocator()) const {
            try {
                MapValue const& mapValue = map.at(key);
                auto castValue = std::any_cast<T>(mapValue);
                return castValue;
            } catch (...) {}

            return allocator();
        }

        /// Returns the value of the specified type, with the specified key
        /// Throws an exception of the value doesn't exist or is the wrong type
        template <class T>
        T& TypeValueAt(Key key) {
            MapValue& value = map.at(key);
            return std::any_cast<T&>(value);
        }

        /// Adds the type value if a tag of this type and id do not exist
        template <class T>
        void TypeAddIfMissing(Key const key, T value = {}) {
            GUARD(!TypeContains<T>(key))
            Add(key, value);
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
            auto& result = *this;

            for (auto& r : rhs.map) {
                this->Add(r.first, r.second);
            }

            return result;
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
            }

            return ss.str();
        }
    };

    /// A set of type tag strings
    /// Example: "enemy", "ghost" tags for an arcade game
    using TypeTagSet = UnorderedSet<String>;
} // namespace PJ
