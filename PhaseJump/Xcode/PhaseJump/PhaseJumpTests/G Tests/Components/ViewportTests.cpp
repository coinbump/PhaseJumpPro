#include "gtest/gtest.h"

#include "MockRenderContext.h"
#include "OrthoCamera.h"
#include "Viewport.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;
using namespace PJTest;

namespace ViewportTests {
    struct SUT {
        SP<World> world;
        OrthoCamera* mainCamera{};
        Viewport* viewport{};

        SUT() {
            world = MAKE<World>();
            auto renderContext = NEW<MockRenderContext>(MockRenderContext::Config{
                .size = { 400, 200 }, .pixelSize = { 400, 200 } });
            world->renderContext = std::move(renderContext);

            mainCamera = &world->AddNode().AddComponent<OrthoCamera>();
            viewport = &world->AddNode().AddComponent<Viewport>();
        }
    };
} // namespace ViewportTests

using namespace ViewportTests;

TEST(Viewport, ContextToWorld) {
    SUT sut;
    sut.viewport->worldSize = Vector2(400, 200);

    // Buffer center maps to owner's world position.
    EXPECT_EQ(Vector3(0, 0, 0), sut.viewport->ContextToWorld(Vector3(200, 100, 0)));
    // Top-left of buffer maps to (-W/2, +H/2) relative to owner (Y-up).
    EXPECT_EQ(Vector3(-200, 100, 0), sut.viewport->ContextToWorld(Vector3(0, 0, 0)));
    // Bottom-right maps to (+W/2, -H/2).
    EXPECT_EQ(Vector3(200, -100, 0), sut.viewport->ContextToWorld(Vector3(400, 200, 0)));
}

TEST(Viewport, ContextToWorldWithOwnerOffset) {
    SUT sut;
    sut.viewport->worldSize = Vector2(400, 200);
    sut.viewport->owner->transform.SetWorldPosition(Vector3(10, 20, 0));

    EXPECT_EQ(Vector3(10, 20, 0), sut.viewport->ContextToWorld(Vector3(200, 100, 0)));
    EXPECT_EQ(Vector3(-190, 120, 0), sut.viewport->ContextToWorld(Vector3(0, 0, 0)));
}

TEST(Viewport, ContextToScreenMapsThroughMainCamera) {
    SUT sut;
    sut.viewport->worldSize = Vector2(400, 200);

    // Buffer center → world (0,0) → screen (200,100) (outer camera's context center).
    EXPECT_EQ(Vector2(200, 100), sut.viewport->ContextToScreen(Vector3(200, 100, 0)));
    // Buffer top-left (0,0) → world (-200, +100) → screen (0, 0) on a 400x200 main camera.
    EXPECT_EQ(Vector2(0, 0), sut.viewport->ContextToScreen(Vector3(0, 0, 0)));
    // Buffer bottom-right → world (+200, -100) → screen (400, 200).
    EXPECT_EQ(Vector2(400, 200), sut.viewport->ContextToScreen(Vector3(400, 200, 0)));
}

TEST(Viewport, ScreenToContextMapsThroughMainCamera) {
    SUT sut;
    sut.viewport->worldSize = Vector2(400, 200);

    // Screen center → world origin → buffer center.
    EXPECT_EQ(Vector3(200, 100, 0), sut.viewport->ScreenToContext(Vector2(200, 100)));
    EXPECT_EQ(Vector3(0, 0, 0), sut.viewport->ScreenToContext(Vector2(0, 0)));
    EXPECT_EQ(Vector3(400, 200, 0), sut.viewport->ScreenToContext(Vector2(400, 200)));
}

TEST(Viewport, ContextToScreenWithOwnerOffset) {
    SUT sut;
    sut.viewport->worldSize = Vector2(400, 200);
    sut.viewport->owner->transform.SetWorldPosition(Vector3(50, 25, 0));

    // Buffer center → world (50, 25) → screen (250, 75) (world (50,25) is 50 right, 25 up from
    // outer camera center, which is pixel (200,100); +50 x, −25 y for Y-flip → (250, 75)).
    EXPECT_EQ(Vector2(250, 75), sut.viewport->ContextToScreen(Vector3(200, 100, 0)));
}

TEST(Viewport, RoundTripsWithOwnerOffset) {
    SUT sut;
    sut.viewport->worldSize = Vector2(400, 200);
    sut.viewport->owner->transform.SetWorldPosition(Vector3(100, -50, 0));

    for (auto sample : VectorList<Vector3>{ Vector3(0, 0, 0), Vector3(200, 100, 0),
                                            Vector3(400, 200, 0), Vector3(37, 219, 0) }) {
        auto screen = sut.viewport->ContextToScreen(sample);
        auto context = sut.viewport->ScreenToContext(screen);
        EXPECT_EQ(sample, context);
    }
}

TEST(Viewport, IsViewportCameraType) {
    Viewport sut;
    EXPECT_EQ(CameraType::Viewport, sut.type);
}
