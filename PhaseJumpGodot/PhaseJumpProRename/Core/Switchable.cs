using Godot;

/*
 * RATING: 5 stars
 * Simple type
 * CODE REVIEW: 12/18/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Can be turned on or off
    /// </summary>
    public interface Switchable
    {
        public bool IsOn { get; set; }
    }
}
