using System;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/20/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// State machine with state timers, to allow a state to last for N seconds
    /// </summary>
    public class EventStateFinish<T> : Event
    {
        public T state { get; protected set; }

        public EventStateFinish(T state, object sentFrom) : base("", sentFrom)
        {
            this.state = state;
        }
    }
}
