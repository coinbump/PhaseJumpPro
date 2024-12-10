#include "gtest/gtest.h"

#include "OrthoCamera.h"
#include "World.h"
#include "WorldNode.h"
#include "MockRenderContext.h"
#include "MockRenderer.h"

using namespace std;
using namespace PJ;
using namespace PJTest;

TEST(World, TestNodePositionsAtRoot) {
    UP<World> world = NEW<World>();

    auto node = MAKE<WorldNode>();
    world->Add(node);
    world->Go();

    EXPECT_EQ(Vector3{}, node->transform.LocalPosition());
    EXPECT_EQ(Vector3{}, node->transform.WorldPosition());

    node->transform.SetLocalPosition(Vector3(1, 1, 0));
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.LocalPosition());
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.WorldPosition());
}

TEST(World, TestNodePositionsAsChild) {
    UP<World> world = NEW<World>();

    auto node = MAKE<WorldNode>();
    auto parentNode = MAKE<WorldNode>();
    parentNode->Add(node);
    world->Add(parentNode);
    world->Go();

    EXPECT_EQ(Vector3{}, node->transform.LocalPosition());
    EXPECT_EQ(Vector3{}, node->transform.WorldPosition());

    node->transform.SetLocalPosition(Vector3(1, 1, 0));
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.LocalPosition());
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.WorldPosition());

    parentNode->transform.SetLocalPosition(Vec3(10, 10, 0));
    EXPECT_EQ(Vector3(1, 1, 0), node->transform.LocalPosition());
    EXPECT_EQ(Vector3(11, 11, 0), node->transform.WorldPosition());
}

//TEST(World, TestRender_ChildInsideParent) {
//    auto camera = MAKE<ReadingCamera>();
//    auto cameraNode = MAKE<WorldNode>();
//    cameraNode->Add(SCAST<SomeWorldComponent>(camera));
//
//    auto parentNode = MAKE<WorldNode>();
//    auto childNode = MAKE<WorldNode>();
//    parentNode->AddEdge(childNode);
//
//    parentNode->transform.position = Vector3(10, 10, 0);
//    childNode->transform.position = Vector3(1, 1, 0);
//    auto mockRenderer = MAKE<MockRenderer>();
//    childNode->Add(SCAST<SomeWorldComponent>(mockRenderer));
//
//    auto sut = MAKE<World>();
//    sut->camera = camera;
//
//    sut->root->AddEdge(parentNode);
//
//    auto renderContext = MAKE<MockRenderContext>();
//    sut->renderContext = renderContext;
//    sut->Render();
//
//    EXPECT_EQ(1, renderContext->renderHistory.size());
//    EXPECT_EQ(1, mockRenderer->renderHistory.size());
//
//    EXPECT_EQ(Vector3(11, 11, 0), mockRenderer->renderHistory[0].position);
//}

