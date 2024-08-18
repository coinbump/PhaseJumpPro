#include "gtest/gtest.h"
#include "Core/Factory.h"

using namespace PJ;
using namespace std;

namespace FactoryTests {
    class TestClass : public Base {
    public:
        using RootBaseType = Base;
        
        int value = 1;

        void foo() {}
    };

    class TestClassWithArgs : public Base {
    public:
        using RootBaseType = Base;

        int intValue = 1;
        String stringValue;

        TestClassWithArgs(int intValue, String stringValue) : intValue(intValue), stringValue(stringValue) {
        }

        void foo() {}
    };
}

using namespace FactoryTests;

TEST(Factory, FactoryNew) {
    Factory<TestClass> factory([]() { return MAKE<TestClass>(); });
    EXPECT_NE(factory.New(), nullptr);
    EXPECT_NE(DCAST<TestClass>(factory.New()), nullptr);
}

TEST(Factory, Factory) {
    auto constructor = [] () -> SP<TestClass> { return MAKE<TestClass>(); };

    Factory<TestClass> factory(constructor);
    EXPECT_NE(factory.New(), nullptr);
    EXPECT_NE(DCAST<TestClass>(factory.New()), nullptr);
}

TEST(Factory, FactoryNewWithArgs) {
    Factory<TestClassWithArgs, int, String> factory([](int intValue, String stringValue) { return MAKE<TestClassWithArgs>(intValue, stringValue); });

    auto value = factory.New(10, "t");
    EXPECT_NE(value, nullptr);

    EXPECT_EQ(10, value->intValue);
    EXPECT_EQ("t", value->stringValue);

    EXPECT_NE(DCAST<TestClassWithArgs>(factory.NewBase(11, "y")), nullptr);
}
