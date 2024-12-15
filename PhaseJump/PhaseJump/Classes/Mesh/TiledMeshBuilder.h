#pragma once

#include "SomeMeshBuilder.h"
#include "UVRect.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/13/24
 */
namespace PJ {
    /**
     Builds uv coordinates and vertices for tiling an internal texture
     Used when the repeated is internal to the whole texture, so we can't use repeat wrap (Example:
     9 slice texture).

     Usage: Use TextureWrap::Repeat if you can. Fallback to this if you need it
     */
    class TiledMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 tileSize{ 1, 1 };
        Vector2 worldSize{ 1, 1 };
        UVRect uvBounds;

        TiledMeshBuilder(Vector2 worldSize, Vector2 tileSize, UVRect uvBounds) :
            worldSize(worldSize),
            tileSize(tileSize),
            uvBounds(uvBounds) {}

        Vector2Int MeshSize() const;

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
