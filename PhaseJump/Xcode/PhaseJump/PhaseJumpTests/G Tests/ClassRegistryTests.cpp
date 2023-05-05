#include "gtest/gtest.h"
#include "Core/Class.h"
#include "ClassRegistry.h"

using namespace PJ;
using namespace std;

namespace ClassRegistryTests {
    class TestType : public PJ::Base {
    };

    class TestClass: public TypeClass<TestType> {
    public:
        int value = 1;

        TestClass() : TypeClass<TestType>("id", MAKE<Factory>([] () -> SP<TestType> { return MAKE<TestType>(); })) {
        }
    };
}

using namespace ClassRegistryTests;

TEST(ClassRegistry, New) {
    SP<TestClass> testClass = MAKE<TestClass>();

    ClassRegistry sut;
    sut["test"] = testClass;

    auto object = sut.NewType<TestType>("test");

    EXPECT_NE(nullptr, object);
    EXPECT_NE(nullptr, DCAST<TestType>(object));
}
