#pragma once

#include "Base.h"
#include "Color.h"
#include "List.h"
#include "SomeRenderCommandModel.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class SomeRenderer;
    class RenderContextModel;
    class SomeRenderEngine;
    class SomeRenderCommandModel;
    class SomeTexture;

    /// Destination model for renders
    class SomeRenderContext : public Base {
    public:
        String id;

        /// Platform native id for render context object
        uint32_t renderId{};

        /// Color used to clear this context for each render pass
        Color clearColor = Color::clear;

        SomeRenderEngine& renderEngine;

        SomeRenderContext(SomeRenderEngine& renderEngine) :
            renderEngine(renderEngine) {}

        virtual ~SomeRenderContext() {}

        /// Makes context current, for renders
        virtual void Bind() = 0;

        /// Clears context with clear color
        virtual void Clear() = 0;

        /// Presents context to display
        virtual void Present() = 0;

        /// @return Returns context's logical size
        virtual Vector2 Size() const = 0;

        /// @return Returns context's size in pixels
        virtual Vector2Int PixelSize() const = 0;

        /// For on demand contexts (texture buffer)
        virtual void Build(Vector2Int size) {}

        /// For viewport contexts (texture buffer)
        virtual SP<SomeTexture> Texture() const {
            return nullptr;
        }
    };
} // namespace PJ
