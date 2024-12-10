#include "gtest/gtest.h"
#include "HFlow.h"
#include "WorldNode.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace HFlowTests {
}

using namespace HFlowTests;

TEST(HFlow, Tests)
{
    auto worldNode = MAKE<WorldNode>();
    auto sut = MAKE<HFlow>(1);

    auto childNode1 = MAKE<WorldNode>();
    auto childNode2 = MAKE<WorldNode>();
    auto childNode3 = MAKE<WorldNode>();

    worldNode->Add(childNode1);
    worldNode->Add(childNode2);
    worldNode->Add(childNode3);

    worldNode->Add(sut);

    sut->LayoutIfNeeded();

    EXPECT_EQ(1.0f, sut->spacing);
    EXPECT_EQ(2.0f, sut->Size().x);
    EXPECT_EQ(1.0f, sut->spacing);
    EXPECT_EQ(Vector2(1.0f, 0) * vecLeft, (Vector2)childNode1->transform.LocalPosition());
    EXPECT_EQ(Vector2{}, (Vector2)childNode2->transform.LocalPosition());
    EXPECT_EQ(Vector2(1.0f, 0) * vecRight, (Vector2)childNode3->transform.LocalPosition());
}
