#ifndef PJBEZIERPATHLAYOUT_H
#define PJBEZIERPATHLAYOUT_H

#include "BezierPath.h"
#include "Vector3.h"
#include "VectorList.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: rethink layouts using composition
    /// Distribute objects along a circle path
    class BezierPathLayout : public SomePathLayout {
    public:
        VectorList<Vector3> controlPoints;

        SP<SomePath> BuildPath() override {
            return SCAST<SomePath>(MAKE<BezierPath>(controlPoints));
        }
    };
} // namespace PJ

#endif
