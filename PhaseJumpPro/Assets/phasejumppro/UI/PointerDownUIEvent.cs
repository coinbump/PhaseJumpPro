using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace PJ
{
    /// <summary>
    /// A pointer down event (mouse, pen, finger)
    /// Wrapper for future portability
    /// </summary>
    public class PointerDownUIEvent : SomePointerUIEvent
    {
        public LocalPosition pressPosition;

        public PointerDownUIEvent(LocalPosition pressPosition)
        {
            this.pressPosition = pressPosition;
        }
    }
}
