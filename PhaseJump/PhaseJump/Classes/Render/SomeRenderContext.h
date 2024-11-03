#pragma once

#include "Base.h"
#include "Color.h"
#include "List.h"
#include "SomeRenderCommandModel.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include <memory>

// /23
namespace PJ {
    class SomeRenderer;
    class RenderContextModel;
    class SomeRenderEngine;
    class SomeRenderCommandModel;
    class SomeTexture;

    /// A render context is how we render graphics via a third party SDK like
    /// SDL or OpenGL
    class SomeRenderContext : public Base {
    public:
        String id;
        uint32_t renderId = 0;

        Color clearColor = Color::clear;

        SP<SomeRenderEngine> renderEngine;

        SomeRenderContext(SP<SomeRenderEngine> renderEngine) :
            renderEngine(renderEngine) {}

        virtual ~SomeRenderContext() {}

        /// Make context current, for renders
        virtual void Bind() = 0;

        /// Clear context with clear color
        virtual void Clear() = 0;

        /// Present context to display
        virtual void Present() = 0;

        virtual Vector2 Size() const = 0;
        virtual Vector2Int PixelSize() const = 0;

        /// Some render contexts are build on demand (texture buffer)
        virtual void Build(Vector2Int size) {}

        /// Some render contexts produce a render texture (texture buffer)
        virtual SP<SomeTexture> Texture() const {
            return nullptr;
        }
    };
} // namespace PJ
