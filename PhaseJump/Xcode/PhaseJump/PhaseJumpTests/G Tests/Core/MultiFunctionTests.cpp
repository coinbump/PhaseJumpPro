#include "gtest/gtest.h"
#include "MultiFunction.h"
#include "StringUtils.h"

using namespace PJ;
using namespace std;

namespace MultiFunctionTests {
    class TestMultiFunction
    {
    public:
        VectorList<int> values;
        MultiFunction<String(VectorList<int>&, int, float)> multiFunction;

        TestMultiFunction()
        {
        }

        void Configure() {
            auto func = [](VectorList<int>& values, int value1, float value2) -> String {
                int total = value1 + (int)value2;

                values.push_back(value1);
                values.push_back((int)value2);

                return MakeString(total);
            };
            IncrementingIdentifier identifier;
            multiFunction.Add(identifier.Next(), func);
            multiFunction.Add(identifier.Next(), func);
        }
    };

    class TestMultiFunctionVoid
    {
    public:
        MultiFunction<void(int, String&)> multiFunction;
        
        TestMultiFunctionVoid()
        {
        }

        void Configure() {
            auto func = [](int value, String& result) {
                result += MakeString(value);
            };
            IncrementingIdentifier identifier;
            multiFunction.Add(identifier.Next(), func);
            multiFunction.Add(identifier.Next(), func);
        }
    };
}

using namespace MultiFunctionTests;

TEST(MultiFunction, Tests)
{
    TestMultiFunction sut;
    sut.Configure();
    sut.multiFunction.reducer = [](String const& first, String next) -> String {
        String firstValue = first;
        return firstValue + next;
    };
    auto result = sut.multiFunction.Run(sut.values, 3, 5.0f);
    EXPECT_EQ("88", result);
    VectorList<int> expectedValues{3, 5, 3, 5};
    EXPECT_EQ(expectedValues, sut.values);
}

TEST(MultiFunction, Remove)
{
    TestMultiFunction sut;
    sut.Configure();
    sut.multiFunction.Remove("1");
    sut.multiFunction.reducer = [](String const& first, String next) -> String {
        String firstValue = first;
        return firstValue + next;
    };
    auto result = sut.multiFunction.Run(sut.values, 3, 5.0f);
    EXPECT_EQ("8", result);
    VectorList<int> expectedValues{3, 5};
    EXPECT_EQ(expectedValues, sut.values);
}

TEST(MultiFunction, Empty)
{
    TestMultiFunction sut;
    
    // This reducer should not be called
    sut.multiFunction.reducer = [](String const& first, String next) -> String {
        String firstValue = first;
        return firstValue + next;
    };
    auto result = sut.multiFunction.Run(sut.values, 3, 5.0f);
    EXPECT_EQ("", result);
    EXPECT_EQ(0, sut.values.size());
    VectorList<int> expectedValues;
    EXPECT_EQ(expectedValues, sut.values);
}

TEST(MultiFunction, VoidSpecialization)
{
    TestMultiFunctionVoid sut;
    sut.Configure();
    int value = 11;
    String result;
    sut.multiFunction.Run(value, result);

    EXPECT_EQ(MakeString(value) + MakeString(value), result);
}

TEST(MultiFunction, VoidRemove)
{
    TestMultiFunctionVoid sut;
    sut.Configure();
    sut.multiFunction.Remove("1");
    int value = 11;
    String result;
    sut.multiFunction.Run(value, result);

    EXPECT_EQ(MakeString(value), result);
}
