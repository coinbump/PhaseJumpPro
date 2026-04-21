#pragma once

#include "SDKIncludes.h"

using namespace PJ;

/// Demonstrates perspective camera renders using a box mesh
class TestPerspectiveScene : public Scene {
public:
    using This = TestPerspectiveScene;

    TestPerspectiveScene();

    // MARK: Scene

    void LoadInto(WorldNode& root) override;

protected:
    float cameraDistance = 0;
    Vector2 windowPixelSize;
    VectorList<Color> faceColors;
    StandardRandom random;

    void AddRandomCube();

    void AddCube(Vector3 position, Vector3 size);
};
