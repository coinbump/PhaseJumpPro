#ifndef PJSOMEPATHLAYOUT_H
#define PJSOMEPATHLAYOUT_H

#include "Angle.h"
#include "SomeLayout.h"
#include "Vector3.h"
#include "VectorList.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class SomePath;

    /// Distribute children along a path
    class SomePathLayout : public SomeLayout {
    public:
        using Base = SomeLayout;

        Vector3 offset;

        bool orientToPath = true;
        float orientDegreeAngle = -90.0f;

        VectorList<float> positions;

        void ApplyLayout() override;

        virtual SP<SomePath> BuildPath() = 0;
    };
} // namespace PJ

#endif
