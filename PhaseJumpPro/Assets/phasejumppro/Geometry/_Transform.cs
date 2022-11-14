using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Needed, because in Unity, Transform is not meant to be created separately from a GameObject.
    /// </summary>
    [System.Serializable]
    public struct _Transform
    {
        public Vector3 position;
        public Vector3 rotation;
        public Vector3 scale;

        public static _Transform defaultTransform = new _Transform(Vector3.zero, Vector3.zero, new Vector3(1.0f, 1.0f, 1.0f));

        public _Transform(Vector3 position, Vector3 rotation, Vector3 scale)
        {
            this.position = position;
            this.rotation = rotation;
            this.scale = scale;
        }
    }
}
