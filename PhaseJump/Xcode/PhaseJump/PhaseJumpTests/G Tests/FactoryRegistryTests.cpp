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
    auto allocator = [] () -> SP<TestClass> { return MAKE<TestClass>(); };

    FactoryRegistry<TestClass> registry;

    auto factory = MAKE<Factory<TestClass>>(allocator);
    registry.map["test"] = factory;

    auto value = ContainsKey(registry.map, "test");
    EXPECT_TRUE(value);

    auto tc = registry.New("test");
    EXPECT_NE(nullptr, tc);
}
