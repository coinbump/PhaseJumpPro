﻿using UnityEditor;
using UnityEngine;
using System.Collections;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Build a circle mesh
    /// </summary>
    public class CircleMeshBuilder : SomeMeshBuilder
    {
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public float radius = 1.0f;

        protected override Vector2 WorldSizeInternal
        {
            get => new Vector2(radius * 2.0f, radius * 2.0f);
            set
            {
                var size = Mathf.Min(value.x, value.y);
                radius = size / 2.0f;
            }
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new CircleMesh(angleStep, radius);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }
    }
}
