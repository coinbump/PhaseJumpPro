#pragma once

#include "Mesh.h"
#include "RGBAColor.h"
#include "SomeRenderer.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    /// Renders between start and end color
    class SimpleGradientRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;

        Color startColor;
        Color endColor;

        SimpleGradientRenderer(Color startColor, Color endColor, Vector2 worldSize);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SimpleGradientRenderer";
        }

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels() override;
    };
} // namespace PJ
