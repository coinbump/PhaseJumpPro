#pragma once

#include "SDKIncludes.h"

using namespace PJ;

/// Exercises nested-viewport rendering in RenderWorldSystem: a parent Viewport (blue) with a
/// nested child Viewport (red). Verifies that the parent viewport doesn't re-render the nested
/// viewport's subtree and that the nested viewport runs before its enclosing viewport.
class TestNestedViewportScene : public Scene {
public:
    using This = TestNestedViewportScene;

    // MARK: Scene

    void LoadInto(WorldNode& root) override;
};
