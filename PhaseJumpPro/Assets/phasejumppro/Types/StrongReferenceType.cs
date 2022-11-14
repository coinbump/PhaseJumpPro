using System;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 11/1/22
 */
namespace PJ
{
    /// <summary>
    /// Holds strong reference to an object
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class StrongReferenceType<T> : SomeReferenceType<T>
    {
        protected T value;

        public T Value
        {
            get => value;
            set
            {
                this.value = value;
            }
        }

        public StrongReferenceType(T value)
        {
            this.value = value;
        }
    }
}
