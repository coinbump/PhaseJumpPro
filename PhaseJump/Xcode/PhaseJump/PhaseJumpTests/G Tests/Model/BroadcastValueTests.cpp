#include "gtest/gtest.h"
#include "BroadcastValue.h"
#include "IntClamp.h"
#include "Macros.h"

using namespace PJ;
using namespace std;

namespace BroadcastValueTests {
    template <class T>
    class TestValue : public BroadcastTransformValue<T>
    {
    public:
        using Base = BroadcastTransformValue<T>;

        TestValue(T value) : Base(value)
        {
        }

        void OnValueChange() override
        {
            Base::OnValueChange();

            changedCount++;
        }

        int changedCount = 0;
    };

    class TestClass
    {
    };
}

using namespace BroadcastValueTests;

TEST(BroadcastValue, Test_BroadcastValue)
{
    auto test = MAKE<TestValue<int>>(0);
    EXPECT_EQ(0, test->Value());
    test->SetValue(10);
    EXPECT_EQ(1, test->changedCount);
    test->SetValue(10);
    EXPECT_EQ(1, test->changedCount);

    test->SetValue(11);
    EXPECT_EQ(2, test->changedCount);
}

TEST(BroadcastValue, Test_NullChange)
{
    auto test = MAKE<TestValue<SP<TestClass>>>(nullptr);
    EXPECT_EQ(nullptr, test->Value());
    test->SetValue(MAKE<TestClass>());
    EXPECT_EQ(1, test->changedCount);
    test->SetValue(MAKE<TestClass>());
    EXPECT_EQ(2, test->changedCount);

    test->SetValue(nullptr);
    EXPECT_EQ(3, test->changedCount);
}

TEST(BroadcastValue, Test_Transform)
{
    auto test = MAKE<TestValue<int>>(0);
    auto transform = MAKE<IntClamp>(0, 10);
    test->transform = transform;

    EXPECT_EQ(0, test->Value());
    test->SetValue(5);
    EXPECT_EQ(5, test->Value());
    test->SetValue(-1);
    EXPECT_EQ(0, test->Value());
    test->SetValue(100);
    EXPECT_EQ(10, test->Value());
}
