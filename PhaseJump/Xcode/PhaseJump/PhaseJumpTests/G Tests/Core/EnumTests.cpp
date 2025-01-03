#include "gtest/gtest.h"

#include "EnumClass.h"
#include "Enum.h"
#include <memory>

using namespace std;
using namespace PJ;

namespace EnumTests {
    enum class TestType {
        One, Two, Three, NotIn
    };

    class TestEnumClass : public EnumClass<TestType> {
    public:
        TestEnumClass() : EnumClass<TestType>("test") {
            MapEnum(TestType::One, "1", "one");
            MapEnum(TestType::Two, "2", "two");
            MapEnum(TestType::Three, "3", "three");
        }
    };

    class TestEnum : public Enum<TestType> {
    public:
        using Base = Enum<TestType>;

        TestEnum() : Base(MAKE<TestEnumClass>()) {
        }
    };
}


using namespace EnumTests;

TEST(Enum, TestEnumClass)
{
    auto sut = MAKE<TestEnumClass>();

    EXPECT_EQ("1", sut->Id(TestType::One));
    EXPECT_EQ("2", sut->Id(TestType::Two));
    EXPECT_EQ("two", sut->Name(TestType::Two));
    EXPECT_EQ("three", sut->Name(TestType::Three));
    EXPECT_EQ(3, sut->AllCases().size());

    EXPECT_EQ(TestType::Two, sut->AllCases()[1]);
}

TEST(Enum, TestEnum)
{
    auto sut = MAKE<TestEnum>();

    sut->value = TestType::Two;

    EXPECT_EQ("2", sut->Id());
    EXPECT_EQ("two", sut->Name());
    EXPECT_EQ(1, sut->Index());
}

TEST(Enum, IndexOf)
{
    auto sut = MAKE<TestEnum>();

    EXPECT_FALSE(sut->EnumClass()->IndexOf(TestType::NotIn));
    EXPECT_EQ(2, sut->EnumClass()->IndexOf(TestType::Three));
}
