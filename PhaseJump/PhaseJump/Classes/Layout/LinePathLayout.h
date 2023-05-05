#ifndef PJLINEPATHLAYOUT_H
#define PJLINEPATHLAYOUT_H

#include "Vector3.h"
#include "SomePathLayout.h"
#include "LinePath.h"

namespace PJ
{
    /// <summary>
    /// Distribute objects along a circle path
    /// </summary>
    class LinePathLayout : public SomePathLayout
    {
    public:
        Vector3 start;
        Vector3 end;

        SP<SomePath> BuildPath() override
        {
            return SCAST<SomePath>(MAKE<LinePath>(start, end));
        }
    };
}

#endif
