#include "gtest/gtest.h"

#include "ReadingCamera.h"
#include "OrthoCamera.h"
#include "World.h"
#include "WorldNode.h"
#include "MockRenderContext.h"
#include "OrthoCamera.h"

using namespace std;
using namespace PJ;
using namespace PJTest;

namespace OrthoCameraTests {
};

TEST(OrthoCamera, CameraExtents) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(200, 100), sut.CameraExtents());
}

TEST(OrthoCamera, SetContentScale) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(400, 200), sut.RenderContextSize());

    sut.SetContentScale(2);
    EXPECT_EQ(Vector2(200, 100), sut.CameraSize());
}

TEST(OrthoCamera, RenderContextSize) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(400, 200), sut.RenderContextSize());
}

TEST(OrthoCamera, WorldToScreenScale) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    sut.SetHalfHeight(200);

    EXPECT_EQ(Vector2(0.5f, 0.5f), sut.WorldToScreenScale());
}

TEST(OrthoCamera, CameraExtentsWithHalfHeight) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    sut.SetHalfHeight(100);

    EXPECT_EQ(Vector2(200, 100), sut.CameraExtents());
}

TEST(OrthoCamera, WorldToScreen) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();

    auto screenZero = sut.WorldToScreen(Vector3::zero);
    EXPECT_EQ(Vector2(200, 100), screenZero);

    auto screenOne = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(201, 99), screenOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1, 0));

    auto screenOneOffset = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(200, 100), screenOneOffset);
}

TEST(OrthoCamera, ScreenToWorld) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();

    auto worldZero = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3::zero, worldZero);

    auto worldOne = sut.ScreenToWorld(Vector2(201, 99));
    EXPECT_EQ(Vector3(1, 1, 0), worldOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1, 0));

    // TODO: is this correct?
    auto worldOneOffset = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3(1, 1, 0), worldOneOffset);
}

TEST(OrthoCamera, WorldToScreenWithHalfHeight) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    sut.SetHalfHeight(50);

    auto screenZero = sut.WorldToScreen(Vector3::zero);
    EXPECT_EQ(Vector2(200, 100), screenZero);

    auto screenOne = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(202, 98), screenOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1, 0));
    auto screenZeroOffset = sut.WorldToScreen(Vector3::zero);
    EXPECT_EQ(Vector2(198, 102), screenZeroOffset);

    auto screenOneOffset = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(200, 100), screenOneOffset);
}

TEST(OrthoCamera, ScreenToWorldWithHalfHeight) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(Vector2(400, 200));
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    sut.SetHalfHeight(50);

    auto worldZero = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3::zero, worldZero);

    auto worldOne = sut.ScreenToWorld(Vector2(202, 98));
    EXPECT_EQ(Vector3(1, 1, 0), worldOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1, 0));

    auto worldZeroOffset = sut.ScreenToWorld(Vector2(198, 102));
    EXPECT_EQ(Vector3::zero, worldZeroOffset);

    auto screenOneOffset = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3(1, 1, 0), screenOneOffset);
}
