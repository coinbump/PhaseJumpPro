#include "gtest/gtest.h"
#include "Core/FactoryRegistry.h"

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
    auto allocator = [] () -> shared_ptr<TestClass> { return make_shared<TestClass>(); };

    FactoryRegistry<TestClass> registry;

    auto factory = make_shared<Factory<TestClass>>(allocator);
    registry["test"] = factory;

    EXPECT_TRUE(registry.ContainsKey("test"));
}
