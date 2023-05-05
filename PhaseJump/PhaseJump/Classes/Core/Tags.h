#ifndef PJTAGS_H_
#define PJTAGS_H_

#include "_Map.h"
#include "_String.h"
#include <set>
#include <any>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/6/22
 */
namespace PJ
{
    /// <summary>
    /// Holds custom attributes (tags) in a dictionary
    /// </summary>
    struct Tags : public Map<String, std::any>
    {
    public:
        using Key = String;
        using MapValue = std::any;

        // Standard
        void Insert(Key key, MapValue value) {
            this->insert({key, value});
        }

        // Convenience (C# style)
        void Add(Key key, MapValue value) {
            Insert(key, value);
        }

        /// <summary>
        /// Creates a value if it doesn't exist
        /// </summary>
        template <class T>
        T SafeValue(Key key) const
        {
            auto value = this->find(key);
            if (value != this->end()) {
                try
                {
                    auto castValue = std::any_cast<T>(value->second);
                    return castValue;
                }
                catch (...)
                {
                }
            }

            return T();
        }

        template <class T>
        std::optional<T> Value(Key key) const
        {
            auto value = this->find(key);
            if (value != this->end()) {
                try
                {
                    auto castValue = std::any_cast<T>(value->second);
                    return std::optional<T>(castValue);
                }
                catch (...)
                {
                }
            }

            return std::nullopt;
        }

        /// <summary>
        /// Returns true if the value exists, and is of the correct type
        /// </summary>
        template <class T>
        bool ContainsTypedValue(Key key) const
        {
            auto value = this->find(key);
            if (value != this->end()) {
                try
                {
                    std::any_cast<T>(value->second);
                    return true;
                }
                catch (...)
                {
                }

                return false;
            }

            return false;
        }
    };

    /// <summary>
    /// Holds a set of type tag strings
    ///
    /// Example: "enemy", "ghost" tags for an arcade game
    /// </summary>
    class TypeTagsSet : std::set<std::string>
    {
    public:
        TypeTagsSet() : std::set<std::string>()
        {
        }

        TypeTagsSet(std::set<std::string> collection) : std::set<std::string>(collection)
        {
        }
    };
}

#endif
