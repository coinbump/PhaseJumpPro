#include "gtest/gtest.h"

#include "WorldNode.h"
#include "NodeHandler.h"
#include "Texture.h"

using namespace std;
using namespace PJ;

namespace NodeHandlerTests {
}

using namespace NodeHandlerTests;

TEST(NodeHandler, Count) {
    auto nodePtr = MAKE<WorldNode>();
    auto& node = *nodePtr;
    size_t componentCount = 0;
    auto sut = MAKE<NodeHandler<>>(NodeHandler<>::Config{
        .func = [&](auto& node) {
            componentCount = node.Components().size();
        }});
    node.Add(sut);
    node.With<WorldComponent>();
    node.With<WorldComponent>();
    
    sut->Run();
    EXPECT_EQ(3, componentCount);
}
