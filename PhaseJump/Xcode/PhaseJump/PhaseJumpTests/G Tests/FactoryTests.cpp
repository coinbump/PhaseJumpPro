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
    Factory<TestClass> factory([]() { return NEW<TestClass>(); });
    EXPECT_NE(factory.Make(), nullptr);
    EXPECT_NE(dynamic_cast<TestClass*>(factory.Make().get()), nullptr);
}

TEST(Factory, Factory) {
    auto constructor = [] () -> UP<TestClass> { return NEW<TestClass>(); };

    Factory<TestClass> factory(constructor);
    EXPECT_NE(factory.Make(), nullptr);
    EXPECT_NE(dynamic_cast<TestClass*>(factory.Make().get()), nullptr);
}

TEST(Factory, FactoryNewWithArgs) {
    Factory<TestClassWithArgs, int, String> factory([](int intValue, String stringValue) { return NEW<TestClassWithArgs>(intValue, stringValue); });

    auto value = factory.Make(10, "t");
    EXPECT_NE(value, nullptr);

    EXPECT_EQ(10, value->intValue);
    EXPECT_EQ("t", value->stringValue);

    EXPECT_NE(dynamic_cast<TestClassWithArgs*>(factory.MakeBase(11, "y").get()), nullptr);
}
