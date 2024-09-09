#pragma once

#include "Mesh.h"
#include "PlaneMeshBuilder.h"
#include "SomeMeshBuilder.h"
#include "UVRect.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Builds uv coordinates and vertices for tiling an internal texture
    /// Used when the repeated is internal to the whole texture, so we can't use
    /// repeat wrap (Example: 9 slice texture) Usage: Use TextureWrap::Repeat if
    /// possible, it's faster. Fallback to this if you need it
    class TiledMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 tileSize{ 1, 1 };
        Vector2 worldSize{ 1.0f, 1.0f };
        UVRect uvBounds;

        TiledMeshBuilder(
            Vector2 worldSize = vec2One, Vector2 tileSize = vec2One, UVRect uvBounds = uvRectOne
        ) :
            worldSize(worldSize),
            tileSize(tileSize),
            uvBounds(uvBounds) {}

        Vector2Int MeshSize() const;
        Mesh BuildMesh() override;
    };
} // namespace PJ
