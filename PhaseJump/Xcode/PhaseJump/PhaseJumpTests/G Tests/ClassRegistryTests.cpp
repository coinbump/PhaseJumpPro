#include "gtest/gtest.h"
#include "Core/Class.h"
#include "ClassRegistry.h"

using namespace PJ;
using namespace std;

namespace ClassRegistryTests {
    class TestType : public Base {
    };

    class TestClass: public TypeClass<TestType> {
    public:
        int value = 1;

        TestClass() : TypeClass<TestType>("id", make_shared<Factory>([] () -> TestType* { return new TestType(); })) {
        }
    };
}

using namespace ClassRegistryTests;

TEST(ClassRegistry, New) {
    auto testClass = new TestClass();

    ClassRegistry sut;
    sut["test"] = shared_ptr<Class>(testClass);

    auto object = sut.NewType<TestType>("test");

    EXPECT_NE(nullptr, object);
    EXPECT_NE(nullptr, dynamic_cast<TestType*>(object));
}
