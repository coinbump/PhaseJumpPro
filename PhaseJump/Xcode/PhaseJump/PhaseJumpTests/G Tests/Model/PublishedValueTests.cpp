#include "gtest/gtest.h"
#include "PublishedValue.h"
#include "IntClamp.h"

using namespace PJ;
using namespace std;

namespace PublishedValueTests {
    template <class T>
    class TestValue : public PublishedValue<T>
    {
    public:
        using Base = PublishedValue<T>;

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

using namespace PublishedValueTests;

TEST(PublishedValue, Test_PublishedValue)
{
    auto test = make_shared<TestValue<int>>(0);
    EXPECT_EQ(0, test->Value());
    test->SetValue(10);
    EXPECT_EQ(1, test->changedCount);
    test->SetValue(10);
    EXPECT_EQ(1, test->changedCount);

    test->SetValue(11);
    EXPECT_EQ(2, test->changedCount);
}

TEST(PublishedValue, Test_NullChange)
{
    auto test = make_shared<TestValue<shared_ptr<TestClass>>>(nullptr);
    EXPECT_EQ(nullptr, test->Value());
    test->SetValue(make_shared<TestClass>());
    EXPECT_EQ(1, test->changedCount);
    test->SetValue(make_shared<TestClass>());
    EXPECT_EQ(2, test->changedCount);

    test->SetValue(nullptr);
    EXPECT_EQ(3, test->changedCount);
}

TEST(PublishedValue, Test_Transform)
{
    auto test = make_shared<TestValue<int>>(0);
    auto transform = make_shared<IntClamp>(0, 10);
    test->transform = transform;

    EXPECT_EQ(0, test->Value());
    test->SetValue(5);
    EXPECT_EQ(5, test->Value());
    test->SetValue(-1);
    EXPECT_EQ(0, test->Value());
    test->SetValue(100);
    EXPECT_EQ(10, test->Value());
}
