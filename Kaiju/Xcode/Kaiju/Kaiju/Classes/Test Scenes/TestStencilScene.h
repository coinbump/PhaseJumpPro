#pragma once

#include "SDKIncludes.h"

using namespace PJ;

/// Stencil test scene to verify render behavior
class TestStencilScene : public Scene {
public:
    using This = TestStencilScene;

    // MARK: Scene

    void LoadInto(WorldNode& root) override;
};
