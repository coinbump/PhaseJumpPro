#ifndef PJRENDERERTOOL_H
#define PJRENDERERTOOL_H

#include "Vector3.h"

namespace PJ
{
    class WorldNode;
    class SomeRenderer;
    class Color;

    /// Handles dispatching to multiple different render types, so a single effect works with multiple renderers
    struct RendererTool {
        SP<SomeRenderer> renderer;

        RendererTool(WorldNode& node);

        std::optional<Vector3> WorldSize() const;
        void SetColor(Color color);
    };
}

#endif
