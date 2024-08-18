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
    class SimpleGradientRenderer : public SomeRenderer, public WorldSizeAble2D {
    public:
        using Base = SomeRenderer;
        RGBAColor startColor;
        RGBAColor endColor;

        MeshRendererModel model;

        SimpleGradientRenderer(Color32 startColor, Color32 endColor, Vector2 worldSize);

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels(RenderIntoModel const& model) override;

        std::optional<Vector3> WorldSize() const override {
            return model.WorldSize();
        }

        // MARK: WorldSizeAble2D

        Vector2 WorldSize2D() const override {
            return model.WorldSize();
        }

        void SetWorldSize2D(Vector2 value) override {
            model.SetWorldSize(value);
        }
    };
} // namespace PJ
