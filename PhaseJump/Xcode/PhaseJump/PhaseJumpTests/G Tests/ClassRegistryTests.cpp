#include "gtest/gtest.h"
#include "TypeClass.h"
#include "ClassRegistry.h"

using namespace PJ;
using namespace std;

namespace ClassRegistryTests {
    class TestType : public Base {
    public:
    };

    class TestClass: public TypeClass<TestType> {
    public:
        int value = 1;

        TestClass() : TypeClass<TestType>("test", []() { return NEW<TestType>(); }) {
        }
    };
    
    class TestTypeWithArguments : public Base {
    public:
        int value{};
        
        TestTypeWithArguments(int value) : value(value) {
        }
    };

    class TestClassWithArguments: public TypeClass<TestTypeWithArguments, int> {
    public:
        int value = 1;

        TestClassWithArguments() : TypeClass<TestTypeWithArguments, int>("test", [](int value) { return NEW<TestTypeWithArguments>(value); }) {
        }
    };
}

using namespace ClassRegistryTests;

TEST(ClassRegistry, NewType) {
    UP<TypeClass<TestType>> testClass = NEW<TestClass>();

    ClassRegistry<TypeClass<TestType>> sut;
    sut.Add(testClass);

    auto object = sut.MakeType<TestType>("test");
    EXPECT_NE(nullptr, object);
}

TEST(ClassRegistry, NewTypeWithArguments) {
    UP<TestClassWithArguments> testClass = NEW<TestClassWithArguments>();

    ClassRegistry<TestClassWithArguments> sut;
    sut.Add(testClass);

    auto object = sut.MakeType<TestTypeWithArguments>("test", 10);
    ASSERT_NE(nullptr, object);
    
    EXPECT_EQ(10, object->value);
}
