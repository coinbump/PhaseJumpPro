using System;
using UnityEngine;

/*
RATING: 5 stars
Simple interface
CODE REVIEW: 11/27/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// A path defines positions along the path from 0-1
    /// </summary>
    public abstract class SomePath : SomeAnimatedValue<Vector3>
    {
        public override Vector3 ValueAt(float progress) { return PositionAt(progress); }
        public abstract Vector3 PositionAt(float progress);
    }
}
