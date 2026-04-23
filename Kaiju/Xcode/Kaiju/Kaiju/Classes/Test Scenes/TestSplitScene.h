#pragma once

#include "SDKIncludes.h"

namespace PJ {
    /// Demonstrates SplitViewLayout: three split views stacked vertically, each subdivided into
    /// two more splits, with random axes and ratios, leaves filled by random colors.
    class TestSplitScene : public Scene {
    public:
        // MARK: Scene

        void LoadInto(WorldNode& root) override;
    };
} // namespace PJ
