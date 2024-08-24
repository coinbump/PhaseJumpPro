#pragma once

#include "Mesh.h"
#include "MeshRendererModel.h"
#include "RGBAColor.h"
#include "SomeRenderer.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Renders a uniform color
    class ColorRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;

        MeshRendererModel model;
        Color color;

        ColorRenderer(Color color, Vector2 worldSize);
        ColorRenderer(SP<RenderMaterial> material, Color color, Vector2 worldSize);

        static SP<RenderMaterial> MakeMaterial(Color color);

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels(RenderContextModel const& model) override;

        // MARK: WorldSizable

        void SetWorldSize(Vector3 value) override {
            model.SetWorldSize(value);
        }

        std::optional<Vector3> WorldSize() const override {
            return Vector3(model.WorldSize());
        }
    };
} // namespace PJ
