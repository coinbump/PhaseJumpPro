#include "gtest/gtest.h"

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

TEST(OrthoCamera, RenderContextSize) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(400, 200), sut.RenderContextSize());
    EXPECT_EQ(Vector2(400, 200), sut.RenderContextPixelSize());
}

TEST(OrthoCamera, RenderContextPixelSize) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(200, 100), sut.RenderContextSize());
    EXPECT_EQ(Vector2(400, 200), sut.RenderContextPixelSize());
}

TEST(OrthoCamera, RenderContextExtents) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(100, 50), sut.RenderContextExtents());
}

TEST(OrthoCamera, CameraExtents) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(200, 100), sut.CameraExtents());
}

TEST(OrthoCamera, CameraExtentsWithHalfHeight) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    sut.SetHalfHeight(50);

    EXPECT_EQ(Vector2(100, 50), sut.CameraExtents());
}

TEST(OrthoCamera, SetContentScale) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(400, 200), sut.CameraSize());

    sut.SetContentScale(2);
    EXPECT_EQ(Vector2(200, 100), sut.CameraSize());
}

TEST(OrthoCamera, WorldToScreenScaleSameSize) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(1.0f, 1.0f), sut.WorldToScreenScale());
}

TEST(OrthoCamera, WorldToScreenScale) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    EXPECT_EQ(Vector2(0.5f, 0.5f), sut.WorldToScreenScale());
}

TEST(OrthoCamera, WorldToScreenSameSize) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();

    auto screenZero = sut.WorldToScreen({});
    EXPECT_EQ(Vector2(200, 100), screenZero);

    auto screenOne = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(201, 99), screenOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1, 0));

    auto screenOneOffset = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(200, 100), screenOneOffset);
}

TEST(OrthoCamera, WorldToScreenScaled) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();

    auto screenZero = sut.WorldToScreen({});
    EXPECT_EQ(Vector2(100, 50), screenZero);

    auto screenOne = sut.WorldToScreen(Vector3(2, 2, 0));
    EXPECT_EQ(Vector2(101, 49), screenOne);

    sut.owner->transform.SetWorldPosition(Vector3(2, 2, 0));

    auto screenOneOffset = sut.WorldToScreen(Vector3(2, 2, 0));
    EXPECT_EQ(Vector2(100, 50), screenOneOffset);
}

TEST(OrthoCamera, WorldToScreenWithHalfHeight) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    sut.SetHalfHeight(50);

    auto screenZero = sut.WorldToScreen({});
    EXPECT_EQ(Vector2(200, 100), screenZero);

    auto screenOne = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(202, 98), screenOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1, 0));
    auto screenZeroOffset = sut.WorldToScreen({});
    EXPECT_EQ(Vector2(198, 102), screenZeroOffset);

    auto screenOneOffset = sut.WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(200, 100), screenOneOffset);
}

TEST(OrthoCamera, ScreenToWorld) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();

    auto worldZero = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3{}, worldZero);

    auto worldOne = sut.ScreenToWorld(Vector2(201, 99));
    EXPECT_EQ(Vector3(1, 1, 0), worldOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1 * vecUp, 0));

    auto worldOneOffset = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3(1, 1, 0), worldOneOffset);
}

TEST(OrthoCamera, ScreenToWorldScaled) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {200, 100}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();

    auto worldZero = sut.ScreenToWorld(Vector2(100, 50));
    EXPECT_EQ(Vector3{}, worldZero);

    auto worldOne = sut.ScreenToWorld(Vector2(101, 49));
    EXPECT_EQ(Vector3(2, 2, 0), worldOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1 * vecUp, 0));

    auto worldOneOffset = sut.ScreenToWorld(Vector2(100, 50));
    EXPECT_EQ(Vector3(1, 1, 0), worldOneOffset);
}

TEST(OrthoCamera, ScreenToWorldWithHalfHeight) {
    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>(MockRenderContext::Config { .size = {400, 200}, .pixelSize = {400, 200}});
    world->renderContext = renderContext;

    auto& sut = world->AddNode().AddComponent<OrthoCamera>();
    sut.SetHalfHeight(50);

    auto worldZero = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3{}, worldZero);

    auto worldOne = sut.ScreenToWorld(Vector2(202, 98));
    EXPECT_EQ(Vector3(1, 1, 0), worldOne);

    sut.owner->transform.SetWorldPosition(Vector3(1, 1 * vecUp, 0));

    auto worldZeroOffset = sut.ScreenToWorld(Vector2(198, 102));
    EXPECT_EQ(Vector3{}, worldZeroOffset);

    auto screenOneOffset = sut.ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3(1, 1, 0), screenOneOffset);
}
