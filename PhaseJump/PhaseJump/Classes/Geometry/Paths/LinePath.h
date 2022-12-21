#ifndef PJLINEPATH_H
#define PJLINEPATH_H

#include "SomePath.h"

/*
 RATING: 5 stars
 Simple utility
 CODE REVIEW: 11/27/22
 */
namespace PJ
{
    /// <summary>
    /// Model that defines a line path
    /// </summary>
    class LinePath : SomePath
    {
    public:
        Vector3 start;
        Vector3 end;

        LinePath(Vector3 start, Vector3 end) : start(start), end(end)
        {
        }

        Vector3 PositionAt(float position) const override
        {
            return start + (end - start) * position;
        }
    };
}

#endif
