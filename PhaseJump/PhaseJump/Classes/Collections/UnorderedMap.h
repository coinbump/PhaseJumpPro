#ifndef PJUNORDEREDMAP_H
#define PJUNORDEREDMAP_H

#include <unordered_map>

namespace PJ {
    template <class Key, class Type> class UnorderedMap : public std::unordered_map<Key, Type>
    {
    public:
        Map() {}
        virtual ~Map() {}

        bool ContainsKey(Key const& key) const {
            return this->find(key) != this->end();
        }

        bool ContainsWhere(std::function<bool(Type const&)> check) const {
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
