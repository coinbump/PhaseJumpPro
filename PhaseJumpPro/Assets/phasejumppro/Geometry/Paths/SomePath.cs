using System;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// A path defines positions along the path from 0-1
    /// </summary>
    public interface SomePath
    {
        public Vector3 PositionAt(float position);
    }
}
