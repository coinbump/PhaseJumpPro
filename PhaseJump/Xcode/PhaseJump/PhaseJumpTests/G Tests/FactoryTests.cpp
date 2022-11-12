#include "gtest/gtest.h"
#include "Core/Factory.h"

using namespace PJ;
using namespace std;

namespace FactoryTests {
    class TestClass {
    public:
        int value = 1;

        void foo() {}
    };
}

using namespace FactoryTests;

TEST(Factory, FactoryNew) {
    FactoryNew<TestClass> factory;
    EXPECT_NE(factory.New(), nullptr);
    EXPECT_NE(dynamic_cast<TestClass*>(factory.New()), nullptr);
}

TEST(Factory, Factory) {
    auto constructor = [] () -> TestClass* { return new TestClass(); };

    Factory<TestClass*> factory(constructor);
    EXPECT_NE(factory.New(), nullptr);
    EXPECT_NE(dynamic_cast<TestClass*>(factory.New()), nullptr);
}
