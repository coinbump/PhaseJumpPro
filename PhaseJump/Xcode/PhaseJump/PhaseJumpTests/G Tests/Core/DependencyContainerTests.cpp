#include "gtest/gtest.h"
#include "DependencyContainer.h"

using namespace PJ;
using namespace std;

namespace DependencyContainerTests {
    struct TestObject {
        int value = 10;
    };
}

using namespace DependencyContainerTests;

TEST(DependencyContainer, CachedDependency)
{
    DependencyContainer sut;

    EXPECT_EQ(0, sut.Count());
    EXPECT_EQ(nullptr, sut.Resolve<int>());
    EXPECT_EQ(nullptr, sut.Resolve<TestObject>());

    int resolveCount = 0;
    auto cachedDependency = MAKE<CachedDependency<TestObject>>([&]() {
        resolveCount++;
        return MAKE<TestObject>();
    });
    sut.Register(cachedDependency);

    EXPECT_EQ(1, sut.Count());
    EXPECT_EQ(cachedDependency.get(), sut.Dependency<TestObject>());
    EXPECT_FALSE(cachedDependency->IsCached());
    EXPECT_EQ(0, resolveCount);

    auto value1 = sut.Resolve<TestObject>();
    EXPECT_EQ(1, resolveCount);
    EXPECT_TRUE(cachedDependency->IsCached());

    auto value2 = sut.Resolve<TestObject>();
    EXPECT_EQ(1, resolveCount);
    EXPECT_EQ(value1, value2);
}
