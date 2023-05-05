#include "gtest/gtest.h"
#include "Core/FactoryRegistry.h"
#include "Macros.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace FactoryRegistryTests {
    class TestClass : public Base {
    public:
        int value = 1;
    };
}

using namespace FactoryRegistryTests;

TEST(FactoryRegistry, Factory) {
    auto allocator = [] () -> SP<TestClass> { return MAKE<TestClass>(); };

    FactoryRegistry<TestClass> registry;

    auto factory = MAKE<Factory<TestClass>>(allocator);
    registry["test"] = factory;

    EXPECT_TRUE(registry.ContainsKey("test"));

    auto tc = registry.New("test");
    EXPECT_NE(nullptr, tc);
}
