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
    class SimpleGradientRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = SimpleGradientRenderer;

        MaterialRendererCore core;

        struct Config {
            Color startColor;
            Color endColor;
            Vector2 worldSize;
        };

        Color startColor;
        Color endColor;

        SimpleGradientRenderer(Config const& config);

        // MARK: Renderer

        RenderModelList RenderModels() override {
            return core.RenderModels();
        }

        Color GetColor() const override {
            return core.GetColor();
        }

        void SetColor(Color color) override {
            core.SetColor(color);
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return core.model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            core.model.SetWorldSize(value);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SimpleGradientRenderer";
        }
    };
} // namespace PJ
