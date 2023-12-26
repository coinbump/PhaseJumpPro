using System;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/20/22
 */
namespace PJ
{
    public class EventStateChange<T> : Event
    {
        public T prevState { get; protected set; }
        public T state { get; protected set; }

        public EventStateChange(T prevState, T state, object sentFrom) : base("", sentFrom)
        {
            this.prevState = prevState;
            this.state = state;
        }
    }
}
