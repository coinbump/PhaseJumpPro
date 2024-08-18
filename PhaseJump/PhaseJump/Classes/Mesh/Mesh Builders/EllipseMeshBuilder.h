#ifndef PJELLIPSEMESHBUILDER_H
#define PJELLIPSEMESHBUILDER_H

#include "Angle.h"
#include "ArcMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/16/23
 */
namespace PJ {
    /// Renders an ellipse
    class EllipseMeshBuilder : public SomeMeshBuilder {
    public:
        Angle angleStep = Angle::DegreesAngle(10.0f);
        Vector2 worldSize = Vector2(1.0f, 1.0f);

        EllipseMeshBuilder(Angle angleStep, Vector2 worldSize) :
            angleStep(angleStep),
            worldSize(worldSize) {}

        Mesh BuildMesh() override {
            return ArcMeshBuilder(
                       Angle::DegreesAngle(0), Angle::DegreesAngle(360.0f), angleStep, worldSize
            )
                .BuildMesh();
        }
    };
} // namespace PJ

#endif
