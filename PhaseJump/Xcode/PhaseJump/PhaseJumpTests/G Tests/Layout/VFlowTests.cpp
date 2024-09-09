#include "gtest/gtest.h"
#include "VFlow.h"
#include "WorldNode.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace VFlowTests {
}

using namespace VFlowTests;

TEST(VFlow, Tests)
{
    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<VFlow>();

    auto childNode1 = MAKE<WorldNode>();
    auto childNode2 = MAKE<WorldNode>();
    auto childNode3 = MAKE<WorldNode>();

    worldNode->Add(childNode1);
    worldNode->Add(childNode2);
    worldNode->Add(childNode3);

    worldNode->Add(sut);

    sut->ApplyLayout();

    EXPECT_EQ(1.0f, sut->spacing);
    EXPECT_EQ(2.0f, sut->Size().y);
    EXPECT_EQ(1.0f, sut->spacing);
    EXPECT_EQ(Vector2(0, 1.0f) * vecUp, (Vector2)childNode1->transform.LocalPosition());
    EXPECT_EQ(vec2Zero, (Vector2)childNode2->transform.LocalPosition());
    EXPECT_EQ(Vector2(0, 1.0f) * vecDown, (Vector2)childNode3->transform.LocalPosition());
}
