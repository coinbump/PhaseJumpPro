#pragma once

#include "WorldComponent.h"

#if FALSE
// Unimplemented. Future feature (see Godo: Viewport)
namespace PJ {
    /// Acts as storage and a render hint in the node graph for building
    /// an offscreen buffer
    /// Multiple cameras can share the same offscreen buffer
    class OffscreenBuffer : public WorldComponent<> {
    public:
        Vector2Int size;

        SP<RenderContext> bufferContext;
    };
} // namespace PJ
#endif
