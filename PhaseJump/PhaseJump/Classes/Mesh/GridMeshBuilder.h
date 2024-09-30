#pragma once

#include "Mesh.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/18/24
 */
namespace PJ {
    /// Builds a grid of lines mesh to display a matrix
    class GridMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 worldSize{ 10.0f, 10.0f };
        Vector2Int gridSize{ 3, 3 };
        float strokeWidth = 2;

        GridMeshBuilder(Vector2 worldSize, Vector2Int gridSize, float strokeWidth = 2) :
            worldSize(worldSize),
            gridSize(gridSize),
            strokeWidth(strokeWidth) {};

        int VerticalLinesCount() const {
            return gridSize.x - 1;
        }

        int HorizontalLinesCount() const {
            return gridSize.y - 1;
        }

        int SegmentCount() const {
            return VerticalLinesCount() + HorizontalLinesCount();
        }

        int MeshVertexCount() const {
            return SegmentCount() * 4;
        }

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
