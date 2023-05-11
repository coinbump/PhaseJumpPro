#include "gtest/gtest.h"

#include "ReadingCamera.h"
#include "OrthoCamera.h"
#include "World.h"
#include "WorldNode.h"
#include "MockRenderContext.h"

using namespace std;
using namespace PJ;
using namespace PJTest;
//
//TEST(Camera, TestReadingCamera) {
//    auto sut = MAKE<ReadingCamera>();
//    auto cameraNode = MAKE<WorldNode>();
//    cameraNode->Add(SCAST<SomeWorldComponent>(sut));
//
//    MockRenderContext renderContext;
//    auto screenZero = sut->WorldToScreen(Vector3::zero, renderContext);
//    EXPECT_EQ(Vector3(0, 0, 0), screenZero);
//
//    auto screenOne = sut->WorldToScreen(Vector3(1, 1, 0), renderContext);
//    EXPECT_EQ(Vector3(1, 1, 0), screenOne);
//
//    cameraNode->transform->position.x = 1.0f;
//    cameraNode->transform->position.y = 1.0f;
//
//    auto screenOneOffset = sut->WorldToScreen(Vector3(1, 1, 0), renderContext);
//    EXPECT_EQ(Vector3(0, 0, 0), screenOneOffset);
//}

namespace CameraTests {
};

TEST(Camera, TestOrtho_WorldToScreen) {
    auto sut = MAKE<OrthoCamera>();
    auto cameraNode = MAKE<WorldNode>();
    cameraNode->AddComponent(sut);

    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>();
    world->renderContext = renderContext;

    world->root->AddChild(cameraNode);
    world->Go();

    auto screenZero = sut->WorldToScreen(Vector3::zero);
    EXPECT_EQ(Vector2(200, 100), screenZero);

    auto screenOne = sut->WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(201, 99), screenOne);

    cameraNode->transform->SetWorldPosition(Vector3(1, 1, 0));

    auto screenOneOffset = sut->WorldToScreen(Vector3(1, 1, 0));
    EXPECT_EQ(Vector2(200, 100), screenOneOffset);
}

TEST(Camera, TestOrtho_ScreenToWorld) {
    auto sut = MAKE<OrthoCamera>();
    auto cameraNode = MAKE<WorldNode>();
    cameraNode->AddComponent(sut);

    auto world = MAKE<World>();
    auto renderContext = MAKE<MockRenderContext>();
    world->renderContext = renderContext;

    world->root->AddChild(cameraNode);
    world->Go();

    auto screenZero = sut->ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3::zero, screenZero);

    auto screenOne = sut->ScreenToWorld(Vector2(201, 99));
    EXPECT_EQ(Vector3(1, 1, 0), screenOne);

    cameraNode->transform->SetWorldPosition(Vector3(1, 1, 0));

    auto screenOneOffset = sut->ScreenToWorld(Vector2(200, 100));
    EXPECT_EQ(Vector3(1, 1, 0), screenOneOffset);
}
