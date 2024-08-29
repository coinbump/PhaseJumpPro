#pragma once

#include "Axis.h"
#include "Bounds.h"
#include "Mesh.h"
#include "RenderModel.h"
#include "SomeRenderContext.h"
#include "WorldComponent.h"
#include "WorldSizeable.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class RenderMaterial;

    class SomeRenderer : public WorldComponent<>, public WorldSizeable {
    public:
        // (OPTIONAL) Material for render
        SP<RenderMaterial> material;

        virtual VectorList<RenderModel> MakeRenderModels() = 0;

        /// Allows custom rendering outside of the render engine
        /// Example: rendering ImGui UI
        virtual void RenderInto(RenderContextModel const& contextModel) {}

        /// Override to set render color
        virtual void SetColor(Color color) {}

        // MARK: Convenience

        VectorList<RenderModel>
        MakeRenderModels(Mesh const& mesh, VectorList<SomeTexture*> textures);

        // MARK: WorldSizeable

        void SetWorldSize(Vector3 value) override {}
    };

    /// Render based on a function (used for imGui)
    class ActionRenderer : public SomeRenderer {
    public:
        std::function<void(RenderContextModel)> render;

        ActionRenderer(std::function<void(RenderContextModel)> render) :
            render(render) {}

        // MARK: SomeRenderer

        void RenderInto(RenderContextModel const& contextModel) override;
    };
} // namespace PJ
