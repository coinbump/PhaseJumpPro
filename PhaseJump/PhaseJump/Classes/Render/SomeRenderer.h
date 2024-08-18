#pragma once

#include "Axis.h"
#include "Bounds.h"
#include "Mesh.h"
#include "RenderModel.h"
#include "SomeRenderContext.h"
#include "WorldComponent.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class RenderMaterial;

    class SomeRenderer : public WorldComponent<> {
    public:
        // (OPTIONAL) Material for render
        SP<RenderMaterial> material;

        virtual VectorList<RenderModel> MakeRenderModels(RenderIntoModel const& model) = 0;

        /// Allows custom rendering outside of the render engine
        /// Example: rendering ImGui UI
        virtual void RenderInto(RenderIntoModel const& model) {}

        /// Override to return intrinsic size of renderer (if any)
        virtual std::optional<Vector3> WorldSize() const {
            return std::nullopt;
        }

        /// Override to set render color
        virtual void SetColor(Color color) {}

        // MARK: Convenience

        VectorList<RenderModel> MakeRenderModels(
            RenderIntoModel const& model, Mesh const& mesh, VectorList<SomeTexture*> textures
        );
    };

    /// Render based on a function (used for imGui)
    class ActionRenderer : public SomeRenderer {
    public:
        std::function<void(RenderIntoModel)> render;

        ActionRenderer(std::function<void(RenderIntoModel)> render) :
            render(render) {}

        // MARK: SomeRenderer

        void RenderInto(RenderIntoModel const& model) override;
    };
} // namespace PJ
