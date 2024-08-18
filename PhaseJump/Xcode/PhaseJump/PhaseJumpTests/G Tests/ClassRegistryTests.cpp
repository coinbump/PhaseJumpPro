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

        TestClass() : TypeClass<TestType>("id", [] () -> SP<TestType> { return MAKE<TestType>(); }) {
        }
    };
}

using namespace ClassRegistryTests;

TEST(ClassRegistry, NewType) {
    SP<TestClass> testClass = MAKE<TestClass>();

    ClassRegistry sut;
    sut.map["test"] = testClass;

    SP<TestType> object = sut.NewType<TestType>("test");

    EXPECT_NE(nullptr, object);
}

TEST(ClassRegistry, NewBase) {
    SP<TestClass> testClass = MAKE<TestClass>();

    ClassRegistry sut;
    sut.map["test"] = testClass;

    auto object = sut.NewBase("test");

    EXPECT_NE(nullptr, object);
    EXPECT_NE(nullptr, DCAST<TestType>(object));
}
