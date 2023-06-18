using System;
using UnityEngine;

/*
RATING: 5 stars
Simple interface
CODE REVIEW: 6/10/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// When the pointer is over the object, IsHovering becomes true
    /// (set by UI system only)
    /// </summary>
    public abstract class SomeHoverGestureHandler : WorldComponent
    {
        public abstract bool IsHovering { set; }
    }
}
