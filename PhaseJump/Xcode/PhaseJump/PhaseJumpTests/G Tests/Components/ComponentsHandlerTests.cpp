#include "gtest/gtest.h"

#include "WorldNode.h"
#include "ComponentsHandler.h"
#include "Texture.h"
// Compile test
#include "NodesRenderProcessor.h"

using namespace std;
using namespace PJ;

namespace ComponentsHandlerTests {
}

using namespace ComponentsHandlerTests;

TEST(ComponentsHandler, CountWithContinue) {
    auto nodePtr = MAKE<WorldNode>();
    auto& node = *nodePtr;
    int componentCount = 0;
    auto componentHandler = MAKE<ComponentsHandler<>>(ComponentsHandler<>::Config{
        .func = [&](auto& component) {
            componentCount++;
            return FinishType::Continue;
        }});
    node.Add(componentHandler);
    node.With<WorldComponent>();
    node.With<WorldComponent>();
    
    componentHandler->Run();
    EXPECT_EQ(2, componentCount);
}

TEST(ComponentsHandler, CountWithFinish) {
    auto nodePtr = MAKE<WorldNode>();
    auto& node = *nodePtr;
    int componentCount = 0;
    auto componentHandler = MAKE<ComponentsHandler<>>(ComponentsHandler<>::Config{
        .func = [&](auto& component) {
            componentCount++;
            return FinishType::Finish;
        }});
    node.Add(componentHandler);
    node.With<WorldComponent>();
    node.With<WorldComponent>();
    
    componentHandler->Run();
    EXPECT_EQ(1, componentCount);
}

TEST(ComponentsHandler, CountWithArguments) {
    auto nodePtr = MAKE<WorldNode>();
    auto& node = *nodePtr;
    int componentCount = 0;
    auto componentHandler = MAKE<ComponentsHandler<int>>(ComponentsHandler<int>::Config{
        .func = [&](auto& component, int value) {
            componentCount += value;
            return FinishType::Continue;
        }});
    node.Add(componentHandler);
    node.With<WorldComponent>();
    node.With<WorldComponent>();
    
    componentHandler->Run(10);
    EXPECT_EQ(20, componentCount);
}
