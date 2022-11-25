using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace PJ
{
	/// <summary>
    /// A pointer down event (mouse, pen, finger)
    /// Wrapper for portability
    /// </summary>
	public class PointerDownUIEvent : SomePointerUIEvent
	{
        public Vector2 localPressPosition;

        public PointerDownUIEvent(Vector2 localPressPosition)
        {
            this.localPressPosition = localPressPosition;
        }
	}
}
