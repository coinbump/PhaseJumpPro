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

        ColorRenderer(Color32 color, Vector2 worldSize);

        void SetWorldSize(Vector3 value) {
            model.SetWorldSize(value);
        }

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels(RenderIntoModel const& model) override;

        std::optional<Vector3> WorldSize() const override {
            return Vector3(model.WorldSize());
        }
    };
} // namespace PJ
