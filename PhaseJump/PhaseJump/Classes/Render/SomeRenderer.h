#ifndef PJSOMERENDERER_H
#define PJSOMERENDERER_H

#include "WorldComponent.h"
#include "SomeRenderContext.h"
#include "Bounds.h"
#include "Mesh.h"
#include <memory>

#include "Axis.h"

namespace PJ
{
    class RenderMaterial;

    class SomeRenderer : public WorldComponent {
    public:
        // (OPTIONAL) Material for render
        SP<RenderMaterial> material;

        virtual void RenderInto(RenderIntoModel model) = 0;

        /// Override to return intrinsic size of renderer (if any)
        virtual std::optional<Vector3> WorldSize() const { return std::nullopt; }

        /// Override to set render color
        virtual void SetColor(Color color) { }
    };

    /// Render based on a function (used for imGui)
    class FuncRenderer : public SomeRenderer {
    public:
        std::function<void(RenderIntoModel)> render;

        FuncRenderer(std::function<void(RenderIntoModel)> render) : render(render) {
        }

        void RenderInto(RenderIntoModel model) override;
    };
}

#endif
