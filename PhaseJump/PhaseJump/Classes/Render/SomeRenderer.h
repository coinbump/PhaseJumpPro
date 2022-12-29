#ifndef PJSOMERENDERER_H
#define PJSOMERENDERER_H

#include "WorldComponent.h"
#include "SomeRenderContext.h"
#include <memory>

namespace PJ
{
    class SomeRenderer : public WorldComponent {
    public:
        virtual void RenderInto(std::shared_ptr<SomeRenderContext> renderContext) = 0;
    };
}

#endif
