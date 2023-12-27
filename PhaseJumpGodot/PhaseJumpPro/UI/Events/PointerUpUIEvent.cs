using System;
using Godot;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 6/10/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// A pointer up event (mouse, pen, finger)
    /// Wrapper for future portability
    /// </summary>
    public class PointerUpUIEvent : SomeUIEvent
    {
        PointerInputButtonModel button;

        public PointerUpUIEvent(PointerInputButtonModel button)
        {
            this.button = button;
        }
    }
}
