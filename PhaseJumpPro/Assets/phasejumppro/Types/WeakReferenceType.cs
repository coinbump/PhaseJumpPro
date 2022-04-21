using System;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 4/11/22
 */
namespace PJ
{
    /// <summary>
    /// Holds weak reference to object
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class WeakReferenceType<T> : SomeReferenceType<T> where T : class
    {
        protected WeakReference<T> weakValue;

        public T Value
        {
            get
            {
                if (null != weakValue)
                {
                    if (weakValue.TryGetTarget(out T target))
                    {
                        return target;
                    }
                }

                return null;
            }

            set
            {
                weakValue = new WeakReference<T>(value);
            }
        }

        public WeakReferenceType(T value)
        {
            weakValue = new WeakReference<T>(value);
        }
    }
}
