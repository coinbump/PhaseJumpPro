#ifndef PJSOMERENDERCONTEXT_H
#define PJSOMERENDERCONTEXT_H

#include "Base.h"
#include "Vector2.h"

namespace PJ
{
    class SomeRenderer;
    class RenderIntoModel;
    
    /// <summary>
    /// A render context is how we render graphics via a third party SDK like SDL or OpenGL
    /// </summary>
    class SomeRenderContext : public Base
    {
    public:
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

        virtual void Render(SomeRenderer& renderer, RenderIntoModel model);
    };
}

#endif
