using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace PJ
{
	/// <summary>
    /// A pointer click event (mouse, pen, finger)
    /// Wrapper for portability
    /// </summary>
	public class PointerClickUIEvent : SomePointerUIEvent
	{
        public Vector2 localPressPosition;
        public InputButton button;
        
        public PointerClickUIEvent(Vector2 localPressPosition, InputButton button)
        {
            this.localPressPosition = localPressPosition;
            this.button = button;
        }
	}
}
