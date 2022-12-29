using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple type
 * CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// A collection of typed-keys
    /// Can be sent in response to user actions and used to trigger events
    /// Example: a tutorial asks a user to do something, but there are multiple user actions (signals) that can satisfy the request
    /// </summary>
    public class Signal<Key>
    {
        protected HashSet<Key> keys = new HashSet<Key>();

        public bool Contains(Key key)
        {
            return keys.Contains(key);
        }

        public void AddKey(Key key)
        {
            keys.Add(key);
        }
    }

    /// <summary>
    /// Event that carries a signal
    /// Useful for responding to categories of behavior
    /// </summary>
    public class EventSignal : Event
    {
        public Signal<string> signal = new Signal<string>();
    }
}
