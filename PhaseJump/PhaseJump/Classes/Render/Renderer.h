#pragma once

#include "MaterialRenderModel.h"
#include "Mesh.h"
#include "RendererModel.h"
#include "SomeRenderContext.h"
#include "SomeRenderModel.h"
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
    class MaterialRenderModel;
    class Texture;

    /// Polymorphic list of render models. The DAG builder consumes this from each renderer
    /// and links the resulting nodes into the RenderPassModel.
    using RenderModelList = VectorList<SP<SomeRenderModel>>;

    /// Produces render models for the render engine to render
    class Renderer : public WorldComponent, public WorldSizeable {
    public:
        using This = Renderer;

        using RenderModelsFunc = std::function<RenderModelList(This&)>;

        RenderModelsFunc renderModelsFunc;

        virtual ~Renderer() {}

        /// @return Models that drive the render
        virtual RenderModelList RenderModels() {
            GUARDR(renderModelsFunc, {})
            return renderModelsFunc(*this);
        }

        /// @return Models emitted after this renderer's children in the DAG. Default to empty.
        /// Used for stencil pop
        virtual RenderModelList PostRenderModels() {
            return {};
        }

        /// @return Returns a calculated size from the given size proposal
        virtual Vector2 CalculateSize(Vector2 proposal) {
            return proposal;
        }

        /// Some renderers have a limited time span (animations)
        virtual bool IsRenderFinished() const {
            return false;
        }

        /// @return Returns the renderer's tint color. Default is white.
        virtual Color GetColor() const {
            return Color::white;
        }

        /// Sets the renderer's tint color. Default is no-op.
        virtual void SetColor(Color color) {}

        /// Sets the alpha of the renderer's color
        void SetAlpha(float value) {
            auto color = GetColor();
            color.a = value;
            SetColor(color);
        }
    };

    /// Shared core for components that render a material and mesh
    class MaterialRendererCore {
    public:
        using This = MaterialRendererCore;
        using BuildMeshFunc = RendererModel::BuildMeshFunc;

        /// Owner component. Used to produce the model matrix for rendered models.
        SomeWorldComponent* owner{};

        /// Defines model for renderer
        RendererModel model;

        MaterialRendererCore(SomeWorldComponent* owner, Vector3 worldSize);

        This& SetOwner(SomeWorldComponent* value) {
            owner = value;
            return *this;
        }

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

        Color GetColor() const {
            return model.GetColor();
        }

        /// Create models to send to the render engine for a render
        RenderModelList RenderModels();

        /// @return Returns a PlanUIFunc that provides standard editor UI for the core
        WorldComponent::PlanUIFunc MakePlanUIFunc();
    };
} // namespace PJ
