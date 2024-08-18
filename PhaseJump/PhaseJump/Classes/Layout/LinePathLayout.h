#ifndef PJLINEPATHLAYOUT_H
#define PJLINEPATHLAYOUT_H

#include "LinePath.h"
#include "SomePathLayout.h"
#include "Vector3.h"

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: rethink layouts using composition
    /// Distribute objects along a circle path
    class LinePathLayout : public SomePathLayout {
    public:
        Vector3 start;
        Vector3 end;

        // TODO: SP-audit
        SP<SomePath> BuildPath() override {
            return SCAST<SomePath>(MAKE<LinePath>(start, end));
        }
    };
} // namespace PJ

#endif
