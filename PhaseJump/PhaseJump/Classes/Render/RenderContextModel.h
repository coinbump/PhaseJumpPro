#pragma once

#include "StringUtils.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class SomeRenderContext;
    class WorldNode;
    class Camera;

    struct RenderContextModel {
        /// Pointer, not reference for model copies
        SomeRenderContext* renderContext{};

        /// Root node, used for z-order sorting
        WorldNode* root{};

        VectorList<WorldNode*> nodes;
        VectorList<Camera*> cameras;
    };
} // namespace PJ
