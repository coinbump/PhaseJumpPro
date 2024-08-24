#pragma once

#include "_String.h"
#include "List.h"
#include "SomeRenderCommandModel.h"
#include "VectorList.h"

namespace PJ {
    class SomeRenderContext;
    class WorldNode;

    struct RenderContextModel {
        SomeRenderContext* renderContext = nullptr;

        VectorList<WorldNode*> const& nodes;

        RenderContextModel(SomeRenderContext* renderContext, VectorList<WorldNode*> const& nodes) :
            renderContext(renderContext),
            nodes(nodes) {}
    };
} // namespace PJ
