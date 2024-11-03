#include "gtest/gtest.h"
#include "TypeClass.h"
#include "ClassRegistry.h"

using namespace PJ;
using namespace std;

namespace ClassRegistryTests {
    class TestType : public Base {
    public:
        using RootBaseType = Base;
    };

    class TestClass: public TypeClass<TestType> {
    public:
        int value = 1;

        TestClass() : TypeClass<TestType>("test", [] () -> SP<TestType> { return MAKE<TestType>(); }) {
        }
    };
}

using namespace ClassRegistryTests;

TEST(ClassRegistry, NewType) {
    UP<SomeClass<Base>> testClass = NEW<TestClass>();

    ClassRegistry sut;
    sut.Add(testClass);

    SP<TestType> object = sut.NewType<TestType>("test");

    EXPECT_NE(nullptr, object);
}

TEST(ClassRegistry, NewBase) {
    UP<SomeClass<Base>> testClass = NEW<TestClass>();

    ClassRegistry sut;
    sut.Add(testClass);

    auto object = sut.NewBase("test");

    EXPECT_NE(nullptr, object);
    EXPECT_NE(nullptr, DCAST<TestType>(object));
}
