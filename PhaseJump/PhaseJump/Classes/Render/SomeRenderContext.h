#pragma once

#include "Base.h"
#include "Color.h"
#include "List.h"
#include "SomeRenderCommandModel.h"
#include "SomeRenderEngine.h"
#include "UnorderedMap.h"
#include "Vector2.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class Renderer;
    class RenderContextModel;
    class SomeRenderEngine;
    class SomeRenderCommandModel;
    class Texture;

    /// Destination model for renders
    class SomeRenderContext : public Base {
    public:
        virtual ~SomeRenderContext() {}

        /// @return Returns platform native id for render context object
        virtual RenderItemId RenderId() const = 0;

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

        /// For on demand contexts (texture buffer): tear down and rebuild at the new size. Default
        /// implementation just calls Build, for contexts that don't need explicit teardown.
        virtual void Resize(Vector2Int size) {
            Build(size);
        }

        /// For viewport contexts (texture buffer)
        virtual SP<Texture> GetTexture() const {
            return nullptr;
        }

        /// Replaces this context's backing texture with texture
        virtual void Swap(SP<Texture> texture) {}
    };

    class RenderContext : public SomeRenderContext {
    public:
        String id;

        /// Platform native id for render context object
        RenderItemId renderId{};

        /// Color used to clear this context for each render pass
        Color clearColor = Color::clear;

        /// Per-context clear behavior. Set to `RenderClearType::None` to preserve the buffer's
        /// contents across render passes (e.g. for accumulating brush strokes in a paint viewport).
        RenderClearType clearType = RenderClearType::Clear;

        SomeRenderEngine& renderEngine;

        RenderContext(SomeRenderEngine& renderEngine) :
            renderEngine(renderEngine) {}

        // MARK: SomeRenderContext

        RenderItemId RenderId() const override {
            return renderId;
        }
    };
} // namespace PJ
