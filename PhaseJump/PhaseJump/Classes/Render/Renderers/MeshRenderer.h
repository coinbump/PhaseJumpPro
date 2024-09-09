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
    class MeshRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;

        /// If set, use this color. If not use colors
        std::optional<Color> color;

        /// Contains 1 color per mesh vertex
        VectorList<RenderColor> colors;

        MeshRenderer(Vector3 worldSize);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "MeshRenderer";
        }

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels() override;

        void SetColor(Color color) override {
            this->color = color;
        }
    };
} // namespace PJ
