#include "gtest/gtest.h"

#include "ReadingCamera.h"
#include "CartesianCamera.h"
#include "WorldNode.h"
#include "MockRenderContext.h"

using namespace std;
using namespace PJ;
using namespace PJTest;

TEST(Camera, TestReadingCamera) {
    auto sut = std::make_shared<ReadingCamera>();
    auto cameraNode = std::make_shared<WorldNode>();
    cameraNode->Add(static_pointer_cast<SomeWorldComponent>(sut));

    MockRenderContext renderContext;
    auto screenZero = sut->WorldToScreen(Vector3::zero, renderContext);
    EXPECT_EQ(Vector3(0, 0, 0), screenZero);

    auto screenOne = sut->WorldToScreen(Vector3(1, 1, 0), renderContext);
    EXPECT_EQ(Vector3(1, 1, 0), screenOne);

    cameraNode->transform->position.x() = 1.0f;
    cameraNode->transform->position.y() = 1.0f;

    auto screenOneOffset = sut->WorldToScreen(Vector3(1, 1, 0), renderContext);
    EXPECT_EQ(Vector3(0, 0, 0), screenOneOffset);
}

TEST(Camera, TestCartesianCamera) {
    auto sut = std::make_shared<CartesianCamera>();
    auto cameraNode = std::make_shared<WorldNode>();
    cameraNode->Add(static_pointer_cast<SomeWorldComponent>(sut));

    MockRenderContext renderContext;
    auto screenZero = sut->WorldToScreen(Vector3::zero, renderContext);
    EXPECT_EQ(Vector3(200, 100, 0), screenZero);

    auto screenOne = sut->WorldToScreen(Vector3(1, 1, 0), renderContext);
    EXPECT_EQ(Vector3(201, 99, 0), screenOne);

    cameraNode->transform->position.x() = 1.0f;
    cameraNode->transform->position.y() = 1.0f;

    auto screenOneOffset = sut->WorldToScreen(Vector3(1, 1, 0), renderContext);
    EXPECT_EQ(Vector3(200, 100, 0), screenOneOffset);
}
