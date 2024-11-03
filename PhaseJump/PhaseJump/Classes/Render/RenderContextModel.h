#pragma once

#include "List.h"
#include "SomeRenderCommandModel.h"
#include "StringUtils.h"
#include "VectorList.h"

// /23
namespace PJ {
    class SomeRenderContext;
    class WorldNode;
    class SomeCamera;

    struct RenderContextModel {
        /// Pointer, not reference for model copies
        SomeRenderContext* renderContext = nullptr;

        /// Root node, used for z-order sorting
        WorldNode* root = nullptr;

        VectorList<WorldNode*> nodes;
        VectorList<SomeCamera*> cameras;

        RenderContextModel(
            SomeRenderContext* renderContext, WorldNode* root, VectorList<WorldNode*> const& nodes,
            VectorList<SomeCamera*> const& cameras
        ) :
            renderContext(renderContext),
            root(root),
            nodes(nodes),
            cameras(cameras) {}
    };
} // namespace PJ
