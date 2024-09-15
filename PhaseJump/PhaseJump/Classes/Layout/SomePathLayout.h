#pragma once

#include "Angle.h"
#include "SomeLayout.h"
#include "Vector3.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    class SomePath;

    /// Distribute children along a path
    class SomePathLayout : public SomeLayout {
    public:
        using Base = SomeLayout;

        /// Offset applied to each child node when layout is applied
        Vector3 offset;

        /// If true, child nodes are oriented to the direction of the path
        bool orientToPath = true;

        /// Angle of rotation for child node from orientation of path
        Angle orientDegreeAngle = Angle::DegreesAngle(90.0f);

        /// (Optional). Normalized positions for each child node (0-1.0)
        /// If empty, each child node will be positioned at equal intervals
        VectorList<float> positions;

        /// Called to build the path for this layout
        virtual SP<SomePath> BuildPath() = 0;

        // MARK: SomeLayout

        void ApplyLayout() override;
    };
} // namespace PJ
