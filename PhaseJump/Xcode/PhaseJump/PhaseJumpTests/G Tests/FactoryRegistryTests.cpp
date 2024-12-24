#include "gtest/gtest.h"
#include "Core/FactoryRegistry.h"
#include "Utils.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace FactoryRegistryTests {
    class TestClass : public Base {
    public:
        using RootBaseType = Base;
        
        int value = 1;
    };
}

using namespace FactoryRegistryTests;

TEST(FactoryRegistry, Factory) {
    auto allocator = [] () -> UP<TestClass> { return NEW<TestClass>(); };

    FactoryRegistry<TestClass> registry;

    registry.map["test"] = NEW<Factory<TestClass>>(allocator);

    auto value = ContainsKey(registry.map, "test");
    EXPECT_TRUE(value);

    auto tc = registry.Make("test");
    EXPECT_NE(nullptr, tc);
}
