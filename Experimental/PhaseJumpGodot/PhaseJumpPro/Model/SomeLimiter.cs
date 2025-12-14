using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 11/26/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// A limiter limits whether or not an event can occur
    /// Owner must send OnUpdate events for timers to work
    /// </summary>
    public abstract class SomeLimiter : Updatable
    {
        public bool IsFinished { get => false; }

        public bool Fire()
        {
            bool result = CanFire();
            if (result)
            {
                OnFire();
            }
            return result;
        }

        public abstract bool CanFire();
        public abstract void OnUpdate(TimeSlice time);
        protected abstract void OnFire();
    }
}
