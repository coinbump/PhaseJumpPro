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
    /// A pointer click event (mouse, pen, finger)
    /// Wrapper for future portability
    /// </summary>
    public class PointerClickUIEvent<Position> : SomePointerUIEvent where Position : SomePosition
    {
        public Position pressPosition;

        public PointerClickUIEvent(Position pressPosition, PointerInputButtonModel button)
        {
            this.pressPosition = pressPosition;
            this.buttonModel = button;
        }
    }
}
