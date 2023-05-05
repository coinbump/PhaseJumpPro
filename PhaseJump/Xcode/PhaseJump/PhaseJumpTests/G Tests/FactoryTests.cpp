#include "gtest/gtest.h"
#include "Core/Factory.h"

using namespace PJ;
using namespace std;

namespace FactoryTests {
    class TestClass : public Base {
    public:
        int value = 1;

        void foo() {}
    };
}

using namespace FactoryTests;

TEST(Factory, FactoryNew) {
    FactoryNew<TestClass> factory;
    EXPECT_NE(factory.New(), nullptr);
    EXPECT_NE(DCAST<TestClass>(factory.New()), nullptr);
}

TEST(Factory, Factory) {
    auto constructor = [] () -> SP<TestClass> { return MAKE<TestClass>(); };

    Factory<TestClass> factory(constructor);
    EXPECT_NE(factory.New(), nullptr);
    EXPECT_NE(DCAST<TestClass>(factory.New()), nullptr);
}
