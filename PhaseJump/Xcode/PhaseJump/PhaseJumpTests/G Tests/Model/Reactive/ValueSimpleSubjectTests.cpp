#include "gtest/gtest.h"
#include "SomeSimpleSubject.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace ValueSimpleSubjectTests {
}

using namespace ValueSimpleSubjectTests;

TEST(ValueSimpleSubject, Test_Lifecycle)
{
    auto sut = MAKE<ValueSimpleSubject<int>>(10);
    EXPECT_FALSE(sut->IsComplete());

    EXPECT_EQ(10, sut->Value());

    sut->Send(12);
    EXPECT_EQ(12, sut->Value());

    List<int> values;

    auto subscription = sut->Receive([&values](int const& value) {
        values.push_back(value);
    });

    sut->Send(1);
    EXPECT_EQ(1, sut->Value());
    sut->Send(2);
    EXPECT_EQ(2, sut->Value());

    List<int> expectedValues{12, 1, 2};
    EXPECT_EQ(expectedValues, values);

    sut->Complete();
    EXPECT_TRUE(sut->IsComplete());

    sut->Send(3);
    EXPECT_EQ(expectedValues, values);
}

TEST(ValueSimpleSubject, Test_SubscriptionExpired)
{
    auto sut = MAKE<ValueSimpleSubject<int>>(10);

    List<int> values;

    auto subscription = sut->Receive([&values](int const& value) {
        values.push_back(value);
    });

    EXPECT_EQ(1, sut->ValidSubscriptions().size());
    subscription.reset();
    EXPECT_EQ(0, sut->ValidSubscriptions().size());

    EXPECT_EQ(1, sut->SubscriptionsCount());
    sut->Clean();
    EXPECT_EQ(0, sut->SubscriptionsCount());

    sut->Send(1);
    EXPECT_EQ(1, sut->Value());

    List<int> expectedValues{10};
    EXPECT_EQ(expectedValues, values);
}

TEST(ValueSimpleSubject, Test_SubscriptionCancelled)
{
    auto sut = MAKE<ValueSimpleSubject<int>>(10);

    List<int> values;

    auto subscription = sut->Receive([&values](int const& value) {
        values.push_back(value);
    });

    EXPECT_EQ(1, sut->ValidSubscriptions().size());
    subscription->Cancel();
    EXPECT_EQ(1, sut->ValidSubscriptions().size());

    EXPECT_EQ(1, sut->SubscriptionsCount());
    sut->Clean();
    EXPECT_EQ(0, sut->SubscriptionsCount());
    EXPECT_EQ(0, sut->ValidSubscriptions().size());
}
