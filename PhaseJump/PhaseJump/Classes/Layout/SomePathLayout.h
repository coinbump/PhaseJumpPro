#pragma once

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

        virtual SP<SomePath> BuildPath() = 0;

        // MARK: SomeLayout

        void ApplyLayout() override;

        virtual Vector3 Size() const override {
            return Vector3::zero;
        };
    };
} // namespace PJ
