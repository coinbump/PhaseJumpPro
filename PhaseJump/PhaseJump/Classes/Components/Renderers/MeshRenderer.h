#pragma once

#include "SomeRenderer.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    class RenderMaterial;
    struct Mesh;

    /// Renders a mesh
    class MeshRenderer : public SomeMaterialRenderer {
    public:
        using Base = SomeMaterialRenderer;

        MeshRenderer(Vector3 worldSize);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "MeshRenderer";
        }
    };
} // namespace PJ
