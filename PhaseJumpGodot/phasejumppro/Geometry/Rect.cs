using System;
using Godot;

/*
 * RATING: 5 stars
 * Simple type
 * CODE REVIEW: 6/8/23
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Bounds of an object in View space (positive Y is down)
    /// </summary>
    [Serializable]
    public struct Rect
    {
        public Vector2 origin;
        public Vector2 size;

        public Rect(Vector2 origin, Vector2 size)
        {
            this.origin = origin;
            this.size = size;
        }
    }
}
