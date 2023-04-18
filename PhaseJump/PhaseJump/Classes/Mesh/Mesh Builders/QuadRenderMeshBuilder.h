#ifndef PJQUADRENDERMESHBUILDER_H
#define PJQUADRENDERMESHBUILDER_H

#include "SomeRenderMeshBuilder.h"
#include "PlaneRenderMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/16/23
 */
namespace PJ
{
    /// <summary>
    /// A plane mesh that is oriented to xy space, and defaults to 1x1 size
    /// </summary>
    class QuadRenderMeshBuilder : public SomeRenderMeshBuilder
    {
    public:
        Vector2Int meshSize{1, 1};
        Vector2 worldSize{1.0f, 1.0f};

        QuadRenderMeshBuilder() {
        }

        QuadRenderMeshBuilder(Vector2Int meshSize, Vector2 worldSize) : meshSize(meshSize), worldSize(worldSize)
        {
        }

        RenderMesh BuildRenderMesh() override
        {
            return PlaneRenderMeshBuilder(meshSize, worldSize, Axis::Y)
                .BuildRenderMesh();
        }
    };
}

#endif
