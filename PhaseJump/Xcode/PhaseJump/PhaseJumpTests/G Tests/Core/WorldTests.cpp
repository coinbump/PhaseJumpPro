#include "gtest/gtest.h"

#include "ReadingCamera.h"
#include "CartesianCamera.h"
#include "World.h"
#include "WorldNode.h"
#include "MockRenderContext.h"
#include "MockRenderer.h"

using namespace std;
using namespace PJ;
using namespace PJTest;

TEST(World, TestRender_ChildInsideParent) {
    auto camera = std::make_shared<ReadingCamera>();
    auto cameraNode = std::make_shared<WorldNode>();
    cameraNode->Add(static_pointer_cast<SomeWorldComponent>(camera));

    auto parentNode = std::make_shared<WorldNode>();
    auto childNode = std::make_shared<WorldNode>();
    parentNode->AddEdge(StandardEdgeModel(), childNode);

    parentNode->transform->position = Vector3(10, 10, 0);
    childNode->transform->position = Vector3(1, 1, 0);
    auto mockRenderer = std::make_shared<MockRenderer>();
    childNode->Add(static_pointer_cast<SomeWorldComponent>(mockRenderer));

    auto sut = std::make_shared<World>();
    sut->camera = camera;

    sut->root->AddEdge(StandardEdgeModel(), parentNode);

    auto renderContext = std::make_shared<MockRenderContext>();
    sut->renderContext = renderContext;
    sut->Render();

    EXPECT_EQ(1, renderContext->renderHistory.size());
    EXPECT_EQ(1, mockRenderer->renderHistory.size());

    EXPECT_EQ(Vector3(11, 11, 0), mockRenderer->renderHistory[0].position);
}
