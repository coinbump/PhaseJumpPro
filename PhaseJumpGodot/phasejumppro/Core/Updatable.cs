using System;

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 12/19/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// An object that can receive time update events
    /// </summary>
    public interface Updatable
    {
        public void OnUpdate(TimeSlice time);
        public bool IsFinished { get; }
    }
}
