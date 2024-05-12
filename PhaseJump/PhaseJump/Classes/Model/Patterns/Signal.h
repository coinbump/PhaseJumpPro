#ifndef PJSIGNAL_H
#define PJSIGNAL_H

#include "_Set.h"
#include <memory>

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// A collection of typed-keys
    /// Can be sent in response to user actions and used to trigger events
    /// Example: a tutorial asks a user to do something, but there are multiple user actions (signals) that can satisfy the request
    /// </summary>
    template <class Key>
    class Signal
    {
    protected:
        Set<Key> keys;

    public:
        bool Contains(Key key) const
        {
            return keys.Contains(key);
        }

        void AddKey(Key key)
        {
            keys.Add(key);
        }
    };

    /// <summary>
    /// Event that carries a signal
    /// Useful for responding to categories of behavior
    /// </summary>
    class SignalEvent : public Event
    {
    public:
        SP<Signal<String>> signal = MAKE<Signal<String>>();
    };
}

#endif
