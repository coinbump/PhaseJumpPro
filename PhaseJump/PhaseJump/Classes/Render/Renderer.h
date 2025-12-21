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
    class Texture;

    /// Produces render models for the render engine to render
    class Renderer : public WorldComponent, public WorldSizeable {
    public:
        using This = Renderer;

        using RenderModelsFunc = std::function<VectorList<RenderModel>(This&)>;

        RenderModelsFunc renderModelsFunc;

        virtual ~Renderer() {}

        /// Create models to send to the render engine for a render
        virtual VectorList<RenderModel> RenderModels() {
            GUARDR(renderModelsFunc, {})
            return renderModelsFunc(*this);
        }

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
    class MaterialRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = MaterialRenderer;
        using BuildMeshFunc = RendererModel::BuildMeshFunc;

        /// Defines model for renderer
        RendererModel model;

        MaterialRenderer(Vector3 worldSize);

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

        // MARK: Renderer

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
