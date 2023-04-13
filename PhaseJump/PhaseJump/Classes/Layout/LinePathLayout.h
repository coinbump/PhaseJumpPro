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

        std::shared_ptr<SomePath> BuildPath() override
        {
            return std::static_pointer_cast<SomePath>(std::make_shared<LinePath>(start, end));
        }
    };
}

#endif
