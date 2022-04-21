/*
 * RATING: 5 stars
 * Simple wrapper
 * CODE REVIEW: 4/8/22
 */
namespace PJ
{
    /// <summary>
    /// Allows values to be passed as nullable into functions
    /// </summary>
    public class Optional<T>
    {
        public T value;

        public Optional(T value)
        {
            this.value = value;
        }
    }
}
