using System;
using UnityEngine;

/*
RATING: 5 stars
Simple model
CODE REVIEW: 11/23/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Needed, because in Unity, Transform is not meant to be created separately from a GameObject.
    /// </summary>
    [System.Serializable]
    public struct GeoTransform
    {
        public Vector3 position;
        public Vector3 rotation;
        public Vector3 scale;

        public static GeoTransform defaultTransform = new GeoTransform(Vector3.zero, Vector3.zero, new Vector3(1.0f, 1.0f, 1.0f));

        public GeoTransform(Vector3 position, Vector3 rotation, Vector3 scale)
        {
            this.position = position;
            this.rotation = rotation;
            this.scale = scale;
        }
    }
}
