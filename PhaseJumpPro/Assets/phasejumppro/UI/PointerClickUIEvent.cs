using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace PJ
{
    /// <summary>
    /// A pointer click event (mouse, pen, finger)
    /// Wrapper for future portability
    /// </summary>
    public class PointerClickUIEvent : SomePointerUIEvent
    {
        public LocalPosition pressPosition;
        public InputButton button;

        public PointerClickUIEvent(LocalPosition pressPosition, InputButton button)
        {
            this.pressPosition = pressPosition;
            this.button = button;
        }
    }
}
