#include "gtest/gtest.h"
#include "MultiFunction.h"
#include "_String.h"

using namespace PJ;
using namespace std;

namespace MultiFunctionTests {
    class TestMultiFunction : public MultiFunction<String(List<int>&, int, float)>
    {
    public:
        List<int> values;

        TestMultiFunction()
        {
        }

        void Configure() {
            auto func = [](List<int>& values, int value1, float value2) -> String {
                int total = value1 + (int)value2;

                values.push_back(value1);
                values.push_back((int)value2);

                return String(total);
            };
            funcs.push_back(func);
            funcs.push_back(func);
        }
    };

    class TestMultiFunctionVoid : public MultiFunction<void(int, String&)>
    {
    public:
        TestMultiFunctionVoid()
        {
        }

        void Configure() {
            auto func = [](int value, String& result) {
                result += String(value);
            };
            funcs.push_back(func);
            funcs.push_back(func);
        }
    };
}

using namespace MultiFunctionTests;

TEST(MultiFunction, Tests)
{
    TestMultiFunction sut;
    sut.Configure();
    sut.reducer = [](std::optional<String> first, String next) -> String {
        String firstValue = first.value_or("");
        return firstValue + next;
    };
    auto result = sut.Run(sut.values, 3, 5.0f);
    EXPECT_EQ("88", result);
    EXPECT_EQ(4, sut.values.size());
    List<int> expectedValues{3, 5, 3, 5};
    EXPECT_EQ(expectedValues, sut.values);
}

TEST(MultiFunction, Empty)
{
    TestMultiFunction sut;
    sut.reducer = [](String const& first, String next) -> String {
        String firstValue = first;
        return firstValue + next;
    };
    auto result = sut.Run(sut.values, 3, 5.0f);
    EXPECT_EQ("", result);
    EXPECT_EQ(0, sut.values.size());
    List<int> expectedValues;
    EXPECT_EQ(expectedValues, sut.values);
}

TEST(MultiFunction, VoidSpecialization)
{
    TestMultiFunctionVoid sut;
    sut.Configure();
    int value = 11;
    String result;
    sut.Run(value, result);

    EXPECT_EQ(String(value) + String(value), result);
}
