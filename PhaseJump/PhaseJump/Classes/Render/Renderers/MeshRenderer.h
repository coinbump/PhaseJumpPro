#pragma once

#include "MeshRendererModel.h"
#include "SomeRenderer.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    class RenderMaterial;
    struct Mesh;

    /// Renders a mesh
    class MeshRenderer : public SomeRenderer {
    public:
        /// Defines mesh and mesh builder func
        MeshRendererModel model;

        /// If set, use this color. If not use colors
        std::optional<Color> color;

        /// Contains 1 color per mesh vertex
        VectorList<RenderColor> colors;

        MeshRenderer(Vector3 worldSize);

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels() override;

        void SetColor(Color color) override {
            this->color = color;
        }

        // MARK: WorldSizeable

        std::optional<Vector3> WorldSize() const override {
            return model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            model.SetWorldSize(value);
        }
    };
} // namespace PJ
