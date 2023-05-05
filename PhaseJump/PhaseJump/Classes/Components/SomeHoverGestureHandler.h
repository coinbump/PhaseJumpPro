#ifndef PJSOMEHOVERGESTUREHANDLER_H
#define PJSOMEHOVERGESTUREHANDLER_H

#include "WorldComponent.h"

/*
 RATING: 5 stars
 Simple interface
 Code review: 5/3/23
 */
namespace PJ
{
    /// <summary>
    /// When the pointer is over the object, IsHovering becomes true
    /// (set by UI system only)
    /// </summary>
    class SomeHoverGestureHandler : public WorldComponent
    {
    public:
        virtual void SetIsHovering(bool value) = 0;
    };
}

#endif
