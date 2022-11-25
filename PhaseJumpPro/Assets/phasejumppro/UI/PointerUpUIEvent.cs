using System;
using UnityEngine.EventSystems;

namespace PJ
{
    /// <summary>
    /// A pointer up event (mouse, pen, finger)
    /// Wraps Unity's types for future portability
    /// </summary>
    public class PointerUpUIEvent : SomePointerUIEvent
    {
        public PointerUpUIEvent(PointerEventData eventData)
        {
        }
    }
}
