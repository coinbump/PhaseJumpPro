#ifndef PJELLIPSERENDERMESHBUILDER_H
#define PJELLIPSERENDERMESHBUILDER_H

#include "ArcRenderMeshBuilder.h"
#include "Angle.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/16/23
 */
namespace PJ
{
    /// <summary>
    /// Renders an ellipse
    /// </summary>
    class EllipseRenderMeshBuilder : public SomeRenderMeshBuilder
    {
    public:
        Angle angleStep = Angle::DegreesAngle(10.0f);
        Vector2 worldSize = Vector2(1.0f, 1.0f);

        EllipseRenderMeshBuilder(Angle angleStep, Vector2 worldSize)
        : angleStep(angleStep),
        worldSize(worldSize)
        {
        }

        RenderMesh BuildRenderMesh() override
        {
            return ArcRenderMeshBuilder(Angle::DegreesAngle(0), Angle::DegreesAngle(360.0f), angleStep, worldSize)
                .BuildRenderMesh();
        }
    };
}

#endif
