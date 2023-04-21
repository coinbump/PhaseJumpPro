#ifndef PJSOMERENDERER_H
#define PJSOMERENDERER_H

#include "WorldComponent.h"
#include "SomeRenderContext.h"
#include "Bounds.h"
#include "RenderMesh.h"
#include <memory>

#include "Axis.h"

namespace PJ
{
    class RenderMaterial;

    class SomeRenderer : public WorldComponent {
    public:
        // (OPTIONAL) Material for render
        std::shared_ptr<RenderMaterial> material;

        virtual void RenderInto(RenderIntoModel model) = 0;
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
