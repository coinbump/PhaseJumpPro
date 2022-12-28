using System;
using UnityEngine;

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
