#ifndef PJRECEPTOR_H
#define PJRECEPTOR_H

#include "Signal.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// A receptor responds to signals. A receptor is activated when the signal matches
    /// a key that the receptor responds to
    /// </summary>
    template <class Key>
    class SomeReceptor
    {
    protected:
        virtual void OnSignalMatch(Signal<Key> signal) = 0;

        Set<Key> locks;

    public:
        void AddLockForKey(Key key)
        {
            locks.Add(key);
        }

        virtual bool IsMatch(Signal<Key> signal)
        {
            bool result = false;

            for (auto key : locks)
            {
                if (signal.Contains(key))
                {
                    result = true;
                    break;
                }
            }

            return result;
        }

        void OnSignal(Signal<Key> signal)
        {
            if (IsMatch(signal))
            {
                OnSignalMatch(signal);
            }
        }

        bool Contains(Key key) const
        {
            return locks.Contains(key);
        }
    };
}

#endif
