#ifndef PJSOMEPATH_H
#define PJSOMEPATH_H

#include "Vector3.h"
#include "SomeAnimatedValue.h"

/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 11/27/22
 */
namespace PJ
{
    /// <summary>
    /// A path defines positions along the path from 0-1
    /// </summary>
    class SomePath
    {
    public:
        virtual Vector3 PositionAt(float progress) const = 0;

        virtual ~SomePath() {}
    };
}

#endif
