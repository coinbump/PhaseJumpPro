#ifndef PJSOMERENDERER_H
#define PJSOMERENDERER_H

#include "WorldComponent.h"
#include "SomeRenderContext.h"
#include <memory>

namespace PJ
{
    class SomeRenderer : public WorldComponent {
    public:
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
