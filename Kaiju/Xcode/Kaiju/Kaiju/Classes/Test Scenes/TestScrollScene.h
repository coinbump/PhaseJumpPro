#pragma once

#include "SDKIncludes.h"

// TODO: This isn't working, investigate
namespace PJ {
    /// Demonstrates ScrollView: a long text body is scrolled by a vertical SliderControl
    /// placed to its right.
    class TestScrollScene : public Scene {
    public:
        // MARK: Scene

        void LoadInto(WorldNode& root) override;

    protected:
        WP<ScrollView> scrollView;
    };
} // namespace PJ
