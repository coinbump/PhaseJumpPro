#pragma once

#include "SDKIncludes.h"

using namespace PJ;

/// Test scene for ImRenderer PushClip/PopClip. Draws a red rectangle clipped by an ellipse
class TestImClipScene : public Scene {
public:
    using This = TestImClipScene;

    // MARK: Scene

    void LoadInto(WorldNode& root) override;
};
