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

        TestClass() : TypeClass<TestType>("id", make_shared<Factory>([] () -> std::shared_ptr<TestType> { return make_shared<TestType>(); })) {
        }
    };
}

using namespace ClassRegistryTests;

TEST(ClassRegistry, New) {
    shared_ptr<TestClass> testClass = make_shared<TestClass>();

    ClassRegistry sut;
    sut["test"] = testClass;

    auto object = sut.NewType<TestType>("test");

    EXPECT_NE(nullptr, object);
    EXPECT_NE(nullptr, dynamic_pointer_cast<TestType>(object));
}
