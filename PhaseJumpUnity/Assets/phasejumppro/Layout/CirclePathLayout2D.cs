using System;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// Distribute objects along a circle path
    /// </summary>
    public class CirclePathLayout2D : SomePathLayout, WorldSizeAble2D
    {
        public float radius = 1.0f;

        public Vector2 WorldSize2D
        {
            get => new Vector2(radius * 2.0f, radius * 2.0f);
            set => radius = Mathf.Min(value.x, value.y) / 2.0f;
        }

        protected override SomePath BuildPath()
        {
            return new CirclePath(radius);
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawCircle(transform.position, radius, renderState);
        }
#endif
    }
}
