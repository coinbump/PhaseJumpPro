#include "gtest/gtest.h"
#include "ObservedValue.h"
#include "Utils.h"
#include "VectorList.h"

using namespace PJ;
using namespace std;

namespace ObservedValueTests {
}

using namespace ObservedValueTests;

TEST(ObservedValue, Lifecycle)
{
    auto sut = MAKE<ObservedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());

    VectorList<int> values;

    sut->SetOnValueChangeFunc(SetOnValueChangeFuncType::Sync, [&](auto value) {
        values.push_back(value);
    });

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());
    sut->SetValue(2);
    EXPECT_EQ(2, sut->Value());

    VectorList<int> expectedValues{12, 2};
    EXPECT_EQ(expectedValues, values);
}

TEST(ObservedValue, OperatorAssign)
{
    auto sut = MAKE<ObservedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    VectorList<int> values;

    sut->SetOnValueChangeFunc(SetOnValueChangeFuncType::Sync, [&](auto value) {
        values.push_back(value);
    });

    *sut = 12;
    EXPECT_EQ(12, sut->Value());
    
    VectorList<int> expectedValues{10, 12};
    EXPECT_EQ(expectedValues, values);
}

TEST(ObservedValue, SetValueMove)
{
    ObservedValue<String> sut("hello");

    String moving = "world";
    sut.SetValue(std::move(moving));
    EXPECT_EQ("world", sut.Value());
}

TEST(ObservedValue, SetValueMoveSameValue)
{
    ObservedValue<String> sut("hello");

    VectorList<String> values;
    sut.SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [&](auto value) {
        values.push_back(value);
    });

    String same = "hello";
    sut.SetValue(std::move(same));
    EXPECT_EQ("hello", sut.Value());
    EXPECT_TRUE(values.empty());
}

TEST(ObservedValue, SetOnValueChangeFuncNone)
{
    auto sut = MAKE<ObservedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    VectorList<int> values;

    sut->SetOnValueChangeFunc(SetOnValueChangeFuncType::None, [&](auto value) {
        values.push_back(value);
    });

    EXPECT_TRUE(values.empty());

    sut->SetValue(20);
    sut->SetValue(30);

    VectorList<int> expectedValues{ 20, 30 };
    EXPECT_EQ(expectedValues, values);
}
