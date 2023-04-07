#ifndef PJSOMEFOCUSCOORDINATOR_H
#define PJSOMEFOCUSCOORDINATOR_H

#include "Vector2.h"

namespace PJ
{
    enum class FocusDirection
    {
        Forward, Back, Up, Down, Left, Right
    };

    /// <summary>
    /// Determines the logic for changing focus from the current focusable to the next
    /// </summary>
    class SomeFocusCoordinator
    {
    public:
        virtual void MoveFocus(Vector2 direction)
        {
//            var angleAxisLimiter = new AngleAxisLimiter2D(4);
//            var limitedAngle = angleAxisLimiter.LimitAngle(new Angle(direction));
//
//            if (limitedAngle == 0f)
//            {
//                MoveFocus(FocusDirection.Up);
//            }
//            else if (limitedAngle == 90.0f)
//            {
//                MoveFocus(FocusDirection.Right);
//            }
//            else if (limitedAngle == 180.0f)
//            {
//                MoveFocus(FocusDirection.Down);
//            }
//            else if (limitedAngle == 270.0f)
//            {
//                MoveFocus(FocusDirection.Left);
//            }
        }

        virtual void MoveFocus(FocusDirection direction) = 0;
    };
}

#endif
