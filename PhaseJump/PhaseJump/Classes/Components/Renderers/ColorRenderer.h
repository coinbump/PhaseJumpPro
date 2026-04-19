#pragma once

#include "Mesh.h"
#include "Renderer.h"
#include "RenderTypes.h"
#include "RGBAColor.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    /// Renders a uniform color
    class ColorRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = ColorRenderer;

        MaterialRendererCore core;

        struct Config {
            Color color;
            Vector2 worldSize;
            SP<RenderMaterial> material;
        };

        ColorRenderer(Config const& config);

        /// Disable dynamic blend mode changes
        void EnableBlend(bool isFeatureEnabled);

        static SP<RenderMaterial> MakeMaterial(RenderOpacityType opacityType);

        // MARK: Renderer

        VectorList<RenderModel> RenderModels() override {
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
            return "ColorRenderer";
        }
    };
} // namespace PJ
