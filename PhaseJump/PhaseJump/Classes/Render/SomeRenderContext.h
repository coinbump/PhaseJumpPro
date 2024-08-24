#pragma once

#include "Base.h"
#include "List.h"
#include "SomeRenderCommandModel.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class SomeRenderer;
    class RenderContextModel;
    class SomeRenderEngine;
    class SomeRenderCommandModel;

    /// A render context is how we render graphics via a third party SDK like
    /// SDL or OpenGL
    class SomeRenderContext : public Base {
    public:
        SP<SomeRenderEngine> renderEngine;

        SomeRenderContext() {}

        virtual ~SomeRenderContext() {}

        // Make context current, for renders
        virtual void Bind() = 0;

        // Clear context with clear color
        virtual void Clear() = 0;

        // Present context to display
        virtual void Present() = 0;

        virtual Vector2 Size() const = 0;
        virtual Vector2Int PixelSize() const = 0;

        virtual void Render(SomeRenderer& renderer, RenderContextModel model);
    };
} // namespace PJ
