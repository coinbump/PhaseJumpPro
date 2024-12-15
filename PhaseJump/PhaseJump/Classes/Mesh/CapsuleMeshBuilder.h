#pragma once

#include "Angle.h"
#include "Axis.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/14/24
 */
namespace PJ {
    /// Builds a horizontal capsule mesh (capsules are rarely vertical)
    class CapsuleMeshBuilder : public SomeMeshBuilder {
    public:
        Axis2D axis = Axis2D::X;
        Vector2 worldSize = { 10, 10 };
        Angle angleStep = Angle::WithDegrees(10);

        struct Config {
            Axis2D axis = Axis2D::X;
            Vector2 worldSize{ 10, 10 };
            Angle angleStep = Angle::WithDegrees(10);
        };

        CapsuleMeshBuilder(Config config);

        float CapRadius() const {
            return worldSize.AxisValueOrthogonal(axis) / 2.0f;
        }

        float FrameAxisLength() const {
            return worldSize.AxisValue(axis) - (CapRadius() * 2.0f);
        }

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
