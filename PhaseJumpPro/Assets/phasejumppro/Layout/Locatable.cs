using UnityEngine;

/*
 * RATING: 5 stars
 * Simple interface
 * CODE REVIEW: 1/14/22
 */
namespace PJ
{
    /// <summary>
    /// An object with a location in some ND space system
    /// </summary>
    public interface Locatable
    {
        Vector3 Location { get; }
    }

    /// <summary>
    /// An object with a normalized position (0-1.0)
    /// </summary>
    public interface NormalPositionable
    {
        float PositionNormal { get; }
    }
}
