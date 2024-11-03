#include "gtest/gtest.h"
#include "Utils.h"
#include <memory>

using namespace std;

namespace DefaultValueTests {
    class Foo {
    public:
        int value = 10;
    };

    struct TestStruct {
    public:
        int valueHasDefault = 1;
        int valueNoDefault;

        UP<Foo> pointerDefault = NEW<Foo>();
        UP<Foo> pointerNoDefault;

        void foo() {}
    };

    class TestClass {
    public:
        int valueHasDefault = 1;
        int valueNoDefault;
        UP<Foo> pointerDefault = NEW<Foo>();
        UP<Foo> pointerNoDefault;

        // Class must have constructor, constructor must be defined outside of class
        // or tests will fail
        TestClass();
    };

    TestClass::TestClass() {
    }
}

using namespace DefaultValueTests;

TEST(DefaultValue, Class_OnStack) {
    Foo foo;
    EXPECT_EQ(10, foo.value);

    TestClass sut;

    // Values without a default are random memory
    EXPECT_EQ(1, sut.valueHasDefault);
    ASSERT_NE(nullptr, sut.pointerDefault);
    EXPECT_EQ(10, sut.pointerDefault->value);
}

TEST(DefaultValue, Class_OnHeap) {
    SP<TestClass> sut = MAKE<TestClass>();

    // Values without a default are random memory
    EXPECT_EQ(1, sut->valueHasDefault);
    ASSERT_NE(nullptr, sut->pointerDefault);
    EXPECT_EQ(10, sut->pointerDefault->value);
}

TEST(DefaultValue, Struct_OnStack) {
    TestStruct sut;

    // Values without a default on the stack are random memory
    EXPECT_EQ(1, sut.valueHasDefault);
    EXPECT_NE(nullptr, sut.pointerDefault);
    EXPECT_EQ(10, sut.pointerDefault->value);
}
