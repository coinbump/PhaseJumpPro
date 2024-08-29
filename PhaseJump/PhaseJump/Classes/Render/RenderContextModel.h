#pragma once

#include "List.h"
#include "SomeRenderCommandModel.h"
#include "StringUtils.h"
#include "VectorList.h"

namespace PJ {
    class SomeRenderContext;
    class WorldNode;
    class SomeCamera;

    struct RenderContextModel {
        /// Pointer, not reference for model copies
        SomeRenderContext* renderContext = nullptr;

        VectorList<WorldNode*> nodes;
        VectorList<SomeCamera*> cameras;

        RenderContextModel(
            SomeRenderContext* renderContext, VectorList<WorldNode*> const& nodes,
            VectorList<SomeCamera*> const& cameras
        ) :
            renderContext(renderContext),
            nodes(nodes),
            cameras(cameras) {}
    };
} // namespace PJ
