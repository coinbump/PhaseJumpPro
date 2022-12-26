/*
 * RATING: 5 stars
 * Simple base class
 * CODE REVIEW: 12/20/22
 * PORTED TO: C++
 */
namespace PJ
{

    /// <summary>
    /// Base class for many framework objects, provides standard behavior.
    /// </summary>
    public class Base
    {
        protected bool didGo = false;

        protected virtual void GoInternal() { }

        // Initiate some behavior
        public virtual void Go()
        {
            if (didGo) { return; }
            didGo = true;

            GoInternal();
        }
    }
}
