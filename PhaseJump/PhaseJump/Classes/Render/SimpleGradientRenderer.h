#pragma once

#include "Mesh.h"
#include "MeshRendererModel.h"
#include "RGBAColor.h"
#include "SomeRenderer.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/10/24
 */
namespace PJ {
    /// Renders between start and end color
    class SimpleGradientRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;
        Color startColor;
        Color endColor;

        MeshRendererModel model;

        SimpleGradientRenderer(Color startColor, Color endColor, Vector2 worldSize);

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels(RenderContextModel const& model) override;

        // MARK: WorldSizeable

        std::optional<Vector3> WorldSize() const override {
            return model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            model.SetWorldSize(value);
        }
    };
} // namespace PJ
