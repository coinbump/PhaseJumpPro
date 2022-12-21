#ifndef PJMAP_H
#define PJMAP_H

#include <map>

/*
 RATING: 5 stars
 Simple utility with unit tests
 CODE REVIEW: 11/10/22
 */
namespace PJ
{
    /// <summary>
    /// Adds utility code to std::map
    /// </summary>
    template <class Key, class Value> class Map : public std::map<Key, Value>
    {
    public:
        Map() {}
        virtual ~Map() {}

        bool ContainsKey(Key const& key) const {
            return this->find(key) != this->end();
        }

        bool ContainsWhere(std::function<bool(Value const&)> check) const {
            for (auto const& pair : *this) {
                auto value = pair.second;

                if (check(value)) {
                    return true;
                }
            }

            return false;
        }
    };
}

#endif
