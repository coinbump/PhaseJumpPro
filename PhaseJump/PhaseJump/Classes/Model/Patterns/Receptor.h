#pragma once

#include "ReceptorSignal.h"
#include "UnorderedSet.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// A receptor responds to signals. A receptor is activated when the signal
    /// matches a key that the receptor responds to
    template <class Key>
    class Receptor {
    protected:
        virtual void OnSignalMatch(ReceptorSignal<Key> const& signal) {
            GUARD(onSignalMatchFunc)
            onSignalMatchFunc(signal);
        }

        UnorderedSet<Key> locks;

    public:
        using OnSignalMatchFunc = std::function<void(ReceptorSignal<Key> const& signal)>;
        OnSignalMatchFunc onSignalMatchFunc;

        void AddLockForKey(Key key) {
            locks.insert(key);
        }

        virtual bool IsMatch(ReceptorSignal<Key> const& signal) {
            bool result = false;

            for (auto& key : locks) {
                if (signal.ContainsKey(key)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        void OnSignal(ReceptorSignal<Key> const& signal) {
            GUARD(IsMatch(signal))
            OnSignalMatch(signal);
        }

        bool Contains(Key key) const {
            return locks.Contains(key);
        }
    };
} // namespace PJ
