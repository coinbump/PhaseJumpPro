#include "gtest/gtest.h"
#include "TypeClass.h"
#include "Void.h"

using namespace PJ;
using namespace std;

namespace TypeClassTests {
    struct TestType {
        using RootBaseType = TestType;

        String value;

        TestType(String value) : value(value) {
        }
    };
}

using namespace TypeClassTests;

TEST(TypeClass, InitWithDefaultConstructor)
{
    TypeClass<TestType> sut("id", []() { return MAKE<TestType>(""); });
    auto object = sut.Make();
    GTEST_ASSERT_NE(nullptr, object);
    EXPECT_EQ("", object->value);
}

TEST(TypeClass, InitWithCustomAllocatr)
{
    TypeClass<TestType> sut("id", []() { return MAKE<TestType>("test"); });
    auto object = sut.Make();
    GTEST_ASSERT_NE(nullptr, object);
    EXPECT_EQ("test", object->value);
}
