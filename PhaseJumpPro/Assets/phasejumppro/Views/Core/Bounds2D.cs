using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple type
 * CODE REVIEW: 11/12/22
 */
namespace PJ
{
    /// <summary>
    /// Bounds of an object in View space (positive Y is down)
    /// </summary>
    [Serializable]
    public struct Bounds2D
    {
        public Vector2 origin;
        public Vector2 size;

        public Bounds2D(Vector2 origin, Vector2 size)
        {
            this.origin = origin;
            this.size = size;
        }
    }
}
