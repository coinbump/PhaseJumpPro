#ifndef PJSOMEPATH_H
#define PJSOMEPATH_H

#include "Geometry/Vector3.h"
#include "Animation/SomeAnimatedValue.h"

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
    class SomePath : public SomeAnimatedValue<Vector3>
    {
    public:
        Vector3 ValueAt(float progress) const override { return PositionAt(progress); }
        virtual Vector3 PositionAt(float progress) const = 0;

        virtual ~SomePath() {}
    };
}

#endif
