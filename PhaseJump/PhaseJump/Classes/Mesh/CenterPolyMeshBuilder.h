#pragma once

#include "Angle.h"
#include "CenterPolyBuilder.h"
#include "Mesh.h"
#include "SomeMeshBuilder.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/14/24
 */
namespace PJ {
    /// A mesh that can be defined by a center point at (0, 0) and a surrounding
    /// path Example: capsule, rounded rectangle, arc, ellipse, circle
    class CenterPolyMeshBuilder : public SomeMeshBuilder {
    public:
        Vector2 worldSize;
        CenterPolyModel model;

        CenterPolyMeshBuilder(Vector2 worldSize, CenterPolyModel model) :
            worldSize(worldSize),
            model(model) {}

        // MARK: SomeMeshBuilder

        Mesh BuildMesh() override;
    };
} // namespace PJ
