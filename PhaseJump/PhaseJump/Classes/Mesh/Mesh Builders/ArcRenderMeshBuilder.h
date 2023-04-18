#ifndef PJARCRENDERMESHBUILDER_H
#define PJARCRENDERMESHBUILDER_H

#include "SomeRenderMeshBuilder.h"
#include "CenterPolyRenderMeshBuilder.h"
#include "Angle.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 4/16/23
 */
namespace PJ
{
    /// <summary>
    /// Renders an arc
    /// </summary>
    class ArcRenderMeshBuilder : public SomeRenderMeshBuilder
    {
    public:
        Angle startAngle = Angle::DegreesAngle(0);
        Angle endAngle = Angle::DegreesAngle(360.0f);
        Angle angleStep = Angle::DegreesAngle(10.0f);
        Vector2 worldSize = Vector2(1.0f, 1.0f);

        ArcRenderMeshBuilder(Angle startAngle, Angle endAngle, Angle angleStep, Vector2 worldSize)
        : startAngle(startAngle),
        endAngle(endAngle),
        angleStep(angleStep),
        worldSize(worldSize)
        {
        }

        int SliceCount() const
        {
            if (0 == angleStep.Degrees())
            {
                // Avoid divide-by-zero
                return 1;
            }

            return (int)std::ceil((endAngle.Degrees() - startAngle.Degrees()) / std::abs(angleStep.Degrees()));
        }

        RenderMesh BuildRenderMesh() override
        {
            CenterPolyRenderMeshBuilder centerPolyMesh;
            centerPolyMesh.AddArcVertices(Vector2::zero, Vector2(worldSize.x() / 2.0f, worldSize.y() / 2.0f), startAngle.Degrees(), endAngle.Degrees(), angleStep.Degrees());
            return centerPolyMesh.BuildRenderMesh();
        }
    };
}

#endif
