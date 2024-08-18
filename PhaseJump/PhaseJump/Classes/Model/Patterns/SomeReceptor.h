#ifndef PJRECEPTOR_H
#define PJRECEPTOR_H

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
    class SomeReceptor {
    protected:
        virtual void OnSignalMatch(Signal<Key> signal) = 0;

        UnorderedSet<Key> locks;

    public:
        void AddLockForKey(Key key) {
            locks.insert(key);
        }

        virtual bool IsMatch(Signal<Key> signal) {
            bool result = false;

            for (auto& key : locks) {
                if (signal.ContainsKey(key)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        void OnSignal(Signal<Key> signal) {
            if (IsMatch(signal)) {
                OnSignalMatch(signal);
            }
        }

        bool Contains(Key key) const {
            return locks.Contains(key);
        }
    };
} // namespace PJ

#endif
