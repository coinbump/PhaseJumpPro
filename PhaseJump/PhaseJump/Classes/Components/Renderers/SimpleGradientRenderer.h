#pragma once

#include "Mesh.h"
#include "Renderer.h"
#include "RGBAColor.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    /// Renders between start and end color
    class SimpleGradientRenderer : public MaterialRenderer {
    public:
        using Base = MaterialRenderer;

        struct Config {
            Color startColor;
            Color endColor;
            Vector2 worldSize;
        };

        Color startColor;
        Color endColor;

        SimpleGradientRenderer(Config const& config);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SimpleGradientRenderer";
        }
    };
} // namespace PJ
