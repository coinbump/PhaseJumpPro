﻿using System;
using Godot;

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
    public partial class CirclePathLayout2D : SomePathLayout, WorldSizeAble2D
    {
        [Export]
        public float radius = 1.0f;

        public Vector2 WorldSize2D
        {
            get => new(radius * 2.0f, radius * 2.0f);
            set => radius = Mathf.Min(value.X, value.Y) / 2.0f;
        }

        protected override SomePath BuildPath()
        {
            return new CirclePath(radius);
        }
    }
}
