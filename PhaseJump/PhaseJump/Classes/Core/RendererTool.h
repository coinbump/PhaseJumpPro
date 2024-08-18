#ifndef PJRENDERERTOOL_H
#define PJRENDERERTOOL_H

#include "Utils.h"
#include "Vector3.h"
#include <optional>

// CODE REVIEW: ?/23
namespace PJ {
    class WorldNode;
    class SomeRenderer;
    struct Color;

    // TODO: do we really need this? Seems pointless in C++
    /// Handles dispatching to multiple different render types, so a single
    /// effect works with multiple renderers
    struct RendererTool {
        SP<SomeRenderer> renderer;

        RendererTool(WorldNode& node);

        std::optional<Vector3> WorldSize() const;
        void SetColor(Color color);
    };
} // namespace PJ

#endif
