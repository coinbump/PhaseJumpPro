#pragma once

#include "SDKIncludes.h"

namespace PJ {
    /// Object canvas app with marquee selection
    class CanvasAppScene : public Scene {
    public:
        // MARK: Scene

        void LoadInto(WorldNode& root) override;
    };
} // namespace PJ
