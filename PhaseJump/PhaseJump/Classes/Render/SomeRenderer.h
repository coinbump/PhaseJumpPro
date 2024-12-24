#pragma once

#include "Mesh.h"
#include "RendererModel.h"
#include "RenderModel.h"
#include "SomeRenderContext.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    class RenderMaterial;
    class RenderContextModel;
    class RenderModel;
    class SomeTexture;

    /// Produces render models for the render engine to render
    class SomeRenderer : public WorldComponent<>, public WorldSizeable {
    public:
        virtual ~SomeRenderer() {}

        /// Create models to send to the render engine for a render
        virtual VectorList<RenderModel> RenderModels() = 0;

        /// @return Returns a calculated size from the given size proposal
        virtual Vector2 CalculateSize(Vector2 proposal) {
            return proposal;
        }

        /// Some renderers have a limited time span (animations)
        virtual bool IsRenderFinished() const {
            return false;
        }
    };

    /// Renders a material and mesh
    class SomeMaterialRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;
        using This = SomeMaterialRenderer;
        using BuildMeshFunc = RendererModel::BuildMeshFunc;

        /// Defines model for renderer
        RendererModel model;

        SomeMaterialRenderer(Vector3 worldSize);

        /// Override to reset internal renderer states
        virtual void Reset() {}

        This& SetBuildMeshFunc(BuildMeshFunc buildMeshFunc) {
            model.SetBuildMeshFunc(buildMeshFunc);
            return *this;
        }

        void SetColor(Color color) {
            model.SetColor(color);
        }

        void SetAlpha(float value) {
            model.SetAlpha(value);
        }

        Color GetColor() {
            return model.GetColor();
        }

        // MARK: SomeRenderer

        /// Create models to send to the render engine for a render
        VectorList<RenderModel> RenderModels() override;

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            model.SetWorldSize(value);
        }
    };
} // namespace PJ
