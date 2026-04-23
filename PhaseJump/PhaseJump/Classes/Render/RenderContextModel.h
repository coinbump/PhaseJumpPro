#pragma once

#include "StringUtils.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class RenderPassModel;
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

        /// DAG of render models for this pass. Built by RenderWorldSystem
        SP<RenderPassModel> renderPass;
    };
} // namespace PJ
