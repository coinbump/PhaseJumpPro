#ifndef PJTILEDMESHBUILDER_H
#define PJTILEDMESHBUILDER_H

#include "SomeMeshBuilder.h"
#include "PlaneMeshBuilder.h"
#include "Mesh.h"
#include "UVRect.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/3/22
 */
namespace PJ
{
    /// <summary>
    /// Builds uv coordinates and vertices for tiling an internal texture
    /// Used when the repeated is internal to the whole texture, so we can't use repeat wrap (Example: 9 slice texture)
    /// Usage: Use TextureWrap::Repeat if possible, it's faster. Fallback to this if you need it
    /// </summary>
    class TiledMeshBuilder : public SomeMeshBuilder
    {
    public:
        Vector2 tileSize{1, 1};
        Vector2 worldSize{1.0f, 1.0f};
        UVRect uvBounds;

        TiledMeshBuilder(Vector2 worldSize = Vector2::one, Vector2 tileSize = Vector2::one, UVRect uvBounds = UVRect::one) : worldSize(worldSize), tileSize(tileSize), uvBounds(uvBounds) {
        }

        Vector2Int MeshSize() const;
        Mesh BuildMesh() override;
    };
}

#endif
