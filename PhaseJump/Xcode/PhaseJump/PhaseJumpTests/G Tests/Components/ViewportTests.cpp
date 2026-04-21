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

TEST(Viewport, LocalToWorldIdentityAtOriginWithUnitScale) {
    SUT sut;
    sut.viewport->worldSize = Vector2(100, 100);

    EXPECT_EQ(Vector3(0, 0, 0), sut.viewport->LocalToWorld(Vector3(0, 0, 0)));
    EXPECT_EQ(Vector3(30, 0, 0), sut.viewport->LocalToWorld(Vector3(30, 0, 0)));
    EXPECT_EQ(Vector3(-20, 40, 0), sut.viewport->LocalToWorld(Vector3(-20, 40, 0)));
}

TEST(Viewport, LocalToWorldAppliesOwnerScale) {
    SUT sut;
    sut.viewport->worldSize = Vector2(100, 100);
    sut.viewport->owner->transform.SetScale(Vector3(2, 2, 1));

    // Owner position stays fixed — scale only stretches around it.
    EXPECT_EQ(Vector3(0, 0, 0), sut.viewport->LocalToWorld(Vector3(0, 0, 0)));
    EXPECT_EQ(Vector3(60, 0, 0), sut.viewport->LocalToWorld(Vector3(30, 0, 0)));
    EXPECT_EQ(Vector3(-40, 80, 0), sut.viewport->LocalToWorld(Vector3(-20, 40, 0)));
}

TEST(Viewport, LocalToWorldAppliesOwnerPositionAndScale) {
    SUT sut;
    sut.viewport->worldSize = Vector2(100, 100);
    sut.viewport->owner->transform.SetWorldPosition(Vector3(10, 5, 0));
    sut.viewport->owner->transform.SetScale(Vector3(2, 3, 1));

    EXPECT_EQ(Vector3(10, 5, 0), sut.viewport->LocalToWorld(Vector3(10, 5, 0)));
    // Offset (30, 0) from owner scales to (60, 0) then shifts by owner pos.
    EXPECT_EQ(Vector3(70, 5, 0), sut.viewport->LocalToWorld(Vector3(40, 5, 0)));
    // Negative offset scales too.
    EXPECT_EQ(Vector3(-10, -25, 0), sut.viewport->LocalToWorld(Vector3(0, -5, 0)));
}

TEST(Viewport, WorldToLocalInvertsLocalToWorld) {
    SUT sut;
    sut.viewport->worldSize = Vector2(100, 100);
    sut.viewport->owner->transform.SetWorldPosition(Vector3(10, 5, 0));
    sut.viewport->owner->transform.SetScale(Vector3(2, 3, 1));

    for (auto sample : VectorList<Vector3>{ Vector3(0, 0, 0), Vector3(30, -20, 0),
                                            Vector3(-45, 17, 0), Vector3(1000, -1000, 0) }) {
        auto world = sut.viewport->LocalToWorld(sample);
        auto roundTrip = sut.viewport->WorldToLocal(world);
        EXPECT_EQ(sample, roundTrip);
    }
}

TEST(Viewport, WorldToLocalSurvivesZeroScale) {
    SUT sut;
    sut.viewport->worldSize = Vector2(100, 100);
    sut.viewport->owner->transform.SetWorldPosition(Vector3(10, 5, 0));
    sut.viewport->owner->transform.SetScale(Vector3(0, 0, 0));

    // Zero scale is treated as identity per-axis to avoid NaN; output collapses onto owner pos.
    auto result = sut.viewport->WorldToLocal(Vector3(123, 456, 0));
    EXPECT_EQ(Vector3(123, 456, 0), result);
}

TEST(Viewport, CameraBaseDefaultsToIdentity) {
    auto world = MAKE<World>();
    auto renderContext =
        NEW<MockRenderContext>(MockRenderContext::Config{ .size = { 400, 200 },
                                                          .pixelSize = { 400, 200 } });
    world->renderContext = std::move(renderContext);

    auto& camera = world->AddNode().AddComponent<OrthoCamera>();
    // OrthoCamera does not override LocalToWorld / WorldToLocal — the base Camera identity
    // behavior applies since ortho's local frame already matches the outer world.
    EXPECT_EQ(Vector3(42, -13, 7), camera.LocalToWorld(Vector3(42, -13, 7)));
    EXPECT_EQ(Vector3(42, -13, 7), camera.WorldToLocal(Vector3(42, -13, 7)));

    // ScreenToLocal falls back to ScreenToContext for cameras whose local frame matches the
    // outer world; LocalToScreen is symmetric.
    EXPECT_EQ(camera.ScreenToContext(Vector2(10, 20)), camera.ScreenToLocal(Vector2(10, 20)));
    EXPECT_EQ(
        camera.ContextToScreen(Vector3(3, 4, 0)), camera.LocalToScreen(Vector3(3, 4, 0))
    );
}

TEST(Viewport, ScreenToLocalRoundTripsThroughOwnerScale) {
    SUT sut;
    sut.viewport->worldSize = Vector2(100, 100);
    sut.viewport->owner->transform.SetScale(Vector3(2, 2, 1));

    // The outer camera is 400x200 with world == screen (1:1). A click at screen (260, 100)
    // maps to outer world (60, 0). The 2x-scaled viewport at origin pulls that back to its
    // local frame at (30, 0).
    EXPECT_EQ(Vector3(30, 0, 0), sut.viewport->ScreenToLocal(Vector2(260, 100)));

    // Round trip: local (30, 0) should map back to screen (260, 100).
    EXPECT_EQ(Vector2(260, 100), sut.viewport->LocalToScreen(Vector3(30, 0, 0)));
}
