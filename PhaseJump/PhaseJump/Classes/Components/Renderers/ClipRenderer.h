#pragma once

#include "Renderer.h"
#include "RenderMaterial.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    /// Pushes a stencil region  and pops it after its world-node descendants render
    class ClipRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = ClipRenderer;

        /// Size of the clip region
        Vector2 worldSize;

        /// Material used to draw into the stencil buffer. Color doesn't matter
        SP<RenderMaterial> stencilMaterial;

        ClipRenderer(Vector2 worldSize) :
            worldSize(worldSize) {}

        // MARK: Renderer

        RenderModelList RenderModels() override;
        RenderModelList PostRenderModels() override;

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return Vector3(worldSize.x, worldSize.y, 0);
        }

        void SetWorldSize(Vector3 value) override {
            worldSize = Vector2(value.x, value.y);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ClipRenderer";
        }
    };
} // namespace PJ
