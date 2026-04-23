#pragma once

#include "MaterialRenderModel.h"
#include "Matrix4x4.h"
#include "Mesh.h"
#include "SomeRenderModel.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    class RenderMaterial;

    /// Pushes a stencil region onto the stencil buffer. Draws that happen inside the matching
    /// Push/Pop pair are clipped to the area covered by mesh
    class StencilPushRenderModel : public SomeRenderModel {
    public:
        using Base = SomeRenderModel;
        using This = StencilPushRenderModel;

        /// Mesh that defines the stencil shape.
        Mesh mesh;

        /// Transform applied to the mesh before stenciling.
        Matrix4x4 matrix;

        /// Material used to draw the mesh into the stencil buffer. The engine masks color
        /// writes during the stencil draw, so the material's color/texture don't matter
        RenderMaterial* material{};

        StencilPushRenderModel() {
            type = RenderModelType::StencilPush;
        }
    };
} // namespace PJ
