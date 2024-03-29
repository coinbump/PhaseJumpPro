﻿using System;
using System.Collections.Generic;
using Godot;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 4/23/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Model that defines a circle path
    /// </summary>
    public class BezierPath : SomePath
    {
        // TODO: Unit tests
        public List<Vector3> controlPoints = new();

        public BezierPath(List<Vector3> controlPoints)
        {
            this.controlPoints = controlPoints;
        }

        public override Vector3 PositionAt(float progress)
        {
            var t = progress;

            if (controlPoints.Count == 3)
            {
                return QuadraticValueFor(t, controlPoints[0], controlPoints[1], controlPoints[2]);
            }
            else if (controlPoints.Count >= 4)
            {
                return CubicValueFor(t, controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]);
            }

            return Vector3.Zero;
        }

        public static Vector3 QuadraticValueFor(float t, Vector3 p0, Vector3 p1, Vector3 p2)
        {
            return p1 + Squared(1.0f - t) * (p0 - p1) + Squared(t) * (p2 - p1);
        }

        public static Vector3 CubicValueFor(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
        {
            return Cubed(1.0f - t) * p0 + 3 * Squared(1.0f - t) * t * p1 + 3 * (1.0f - t) * Squared(t) * p2 + Cubed(t) * p3;
        }

        protected static float Squared(float value)
        {
            return value * value;
        }

        protected static float Cubed(float value)
        {
            return value * value * value;
        }
    }
}
