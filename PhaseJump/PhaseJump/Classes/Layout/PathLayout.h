#pragma once

#include "Angle.h"
#include "Layout.h"
#include "SomePath.h"
#include "Vector3.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Distribute children along a path
    class PathLayout : public Layout {
    public:
        using Base = Layout;
        using BuildPathFunc = std::function<UP<SomePath>()>;

        /// Builds the path
        BuildPathFunc buildPathFunc;

        /// Offset applied to each child node when layout is applied
        Vector3 offset;

        /// If true, child nodes are oriented to the direction of the path
        bool orientToPath = true;

        /// Angle of rotation for child node from orientation of path
        Angle orientDegreeAngle = Angle::WithDegrees(90.0f);

        /// (Optional). Normalized positions for each child node (0-1.0)
        /// If empty, each child node will be positioned at equal intervals
        VectorList<float> positions;

        /// Called to build the path for this layout
        virtual UP<SomePath> BuildPath() {
            GUARDR(buildPathFunc, {})
            return buildPathFunc();
        }

        // MARK: Layout

        void ApplyLayout() override;
    };
} // namespace PJ
