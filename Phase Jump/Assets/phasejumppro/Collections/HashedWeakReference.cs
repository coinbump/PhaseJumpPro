using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Solid pattern, with unit tests
 * CODE REVIEW: 3/8/22
 */
namespace PJ
{
    /// <summary>
    /// Used to store weak references in a HashSet. 
    /// (WeakReference won't work because it compares the WeakReference object, not the target)
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class HashedWeakReference<T> where T : class
    {
        protected WeakReference<T> reference;

        public WeakReference<T> Reference {
            get {
                return reference;
            }
        }

        public HashedWeakReference(T reference)
        {
            this.reference = new WeakReference<T>(reference);
        }

        public override int GetHashCode()
        {
            if (reference.TryGetTarget(out T target))
            {
                return target.GetHashCode();
            }
            return 0;
        }

        public override bool Equals(Object obj)
        {
            if (obj is HashedWeakReference<T>)
            {
                var comparison = obj as HashedWeakReference<T>;
                if (reference.TryGetTarget(out T target) && comparison.reference.TryGetTarget(out T comparisonTarget))
                {
                    return target == comparisonTarget;
                }
            }
            return false;
        }
    }
}
