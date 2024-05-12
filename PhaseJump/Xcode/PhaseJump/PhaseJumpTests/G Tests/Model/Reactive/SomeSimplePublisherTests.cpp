#include "gtest/gtest.h"
#include "SomeSimplePublisher.h"

using namespace PJ;
using namespace std;

namespace SomeSimplePublisherTests {
}

using namespace SomeSimplePublisherTests;

TEST(SomeSimplePublisher, Test_Complete)
{
    auto sut = MAKE<SomeSimplePublisher<int>>();
    EXPECT_FALSE(sut->IsComplete());

    sut->Complete();
    EXPECT_TRUE(sut->IsComplete());
}
