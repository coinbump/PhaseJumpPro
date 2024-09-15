#pragma once

#include "Signal.h"
#include "UnorderedSet.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// A receptor responds to signals. A receptor is activated when the signal
    /// matches a key that the receptor responds to
    template <class Key>
    class Receptor {
    protected:
        virtual void OnSignalMatch(Signal<Key> const& signal) {
            GUARD(onSignalMatchFunc)
            onSignalMatchFunc(signal);
        }

        UnorderedSet<Key> locks;

    public:
        using OnSignalMatchFunc = std::function<void(Signal<Key> const& signal)>;
        OnSignalMatchFunc onSignalMatchFunc;

        void AddLockForKey(Key key) {
            locks.insert(key);
        }

        virtual bool IsMatch(Signal<Key> const& signal) {
            bool result = false;

            for (auto& key : locks) {
                if (signal.ContainsKey(key)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        void OnSignal(Signal<Key> const& signal) {
            GUARD(IsMatch(signal))
            OnSignalMatch(signal);
        }

        bool Contains(Key key) const {
            return locks.Contains(key);
        }
    };
} // namespace PJ
