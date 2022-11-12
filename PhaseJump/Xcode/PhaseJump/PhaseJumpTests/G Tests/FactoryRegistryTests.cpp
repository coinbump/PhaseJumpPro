#include "gtest/gtest.h"
#include "Core/FactoryRegistry.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace FactoryRegistryTests {
    class TestClass {
    public:
        int value = 1;
    };
}

using namespace FactoryRegistryTests;

TEST(FactoryRegistry, Factory) {
    auto constructor = [] () -> TestClass* { return new TestClass(); };

    FactoryRegistry<TestClass> registry;

    auto factory = make_shared<Factory<TestClass*>>(constructor);
    registry["test"] = factory;

    EXPECT_TRUE(registry.ContainsKey("test"));
}
