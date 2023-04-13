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

        std::shared_ptr<SomePath> BuildPath() override
        {
            return static_pointer_cast<SomePath>(std::make_shared<BezierPath>(controlPoints));
        }
    };
}

#endif
