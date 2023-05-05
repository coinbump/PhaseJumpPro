#ifndef PJSOMEPATHLAYOUT_H
#define PJSOMEPATHLAYOUT_H

#include "Angle.h"
#include "Vector3.h"
#include "VectorList.h"
#include "SomeLayout.h"
#include <memory>

namespace PJ
{
    class SomePath;

    /// <summary>
    /// Distribute children along a path
    /// </summary>
    class SomePathLayout : public SomeLayout
    {
    public:
        using Base = SomeLayout;

        Vector3 offset;

        bool orientToPath = true;
        float orientDegreeAngle = -90.0f;

        VectorList<float> positions;

        void ApplyLayout() override;

        virtual SP<SomePath> BuildPath() = 0;
    };
}

#endif
