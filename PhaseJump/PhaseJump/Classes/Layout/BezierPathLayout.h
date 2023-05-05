#ifndef PJBEZIERPATHLAYOUT_H
#define PJBEZIERPATHLAYOUT_H

#include "VectorList.h"
#include "Vector3.h"
#include "BezierPath.h"
#include <memory>

namespace PJ
{
    /// <summary>
    /// Distribute objects along a circle path
    /// </summary>
    class BezierPathLayout : public SomePathLayout
    {
    public:
        VectorList<Vector3> controlPoints;

        SP<SomePath> BuildPath() override
        {
            return SCAST<SomePath>(MAKE<BezierPath>(controlPoints));
        }
    };
}

#endif
