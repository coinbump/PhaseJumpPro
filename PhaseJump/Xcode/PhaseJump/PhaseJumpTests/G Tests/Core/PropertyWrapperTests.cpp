#include "gtest/gtest.h"
#include "World.h"
#include "WorldNode.h"
#include "PropertyWrapper.h"

using namespace PJ;
using namespace std;

namespace PropertyWrapperTests {
}

using namespace PropertyWrapperTests;

TEST(PropertyWrapper, Test)
{
    PropertyWrapper<int> sut([](int const& value) { return value + 10; }, [](int const& newValue) { return newValue * 2; });

    EXPECT_EQ(10, (int)sut);

    sut = 4;
    EXPECT_EQ(18, (int)sut);
}
