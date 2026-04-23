#pragma once

#include "Renderer.h"
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
    class MeshRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = MeshRenderer;

        MaterialRendererCore core;

        MeshRenderer(Vector3 worldSize);

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
            return "MeshRenderer";
        }
    };
} // namespace PJ
