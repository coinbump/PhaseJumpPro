#pragma once

#include "SomeSignal.h"
#include "StringUtils.h"
#include "UnorderedSet.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// A collection of typed-keys
    /// Can be sent in response to user actions and used to trigger events
    /// Example: a tutorial asks a user to do something, but there are multiple
    /// user actions (signals) that can satisfy the request
    template <class Key>
        requires std::equality_comparable<Key>
    class Signal : public SomeSignal {
    protected:
        UnorderedSet<Key> keys;

    public:
        bool ContainsKey(Key key) const {
            return Contains(keys, key);
        }

        UnorderedSet<Key>& Keys() {
            return keys;
        }

        UnorderedSet<Key> const& Keys() const {
            return keys;
        }

        void AddKey(Key key) {
            keys.insert(key);
        }
    };
} // namespace PJ
