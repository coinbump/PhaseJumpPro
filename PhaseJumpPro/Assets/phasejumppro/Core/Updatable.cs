using System;

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
    /// <summary>
    /// An object that can receive time update events
    /// </summary>
    public interface Updatable
    {
        public void OnUpdate(TimeSlice time);
    }
}
