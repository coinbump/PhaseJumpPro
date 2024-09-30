#include "gtest/gtest.h"
#include "MockRandom.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace MockRandomTests {
}

using namespace MockRandomTests;

TEST(MockRandom, Test) {
    MockRandom sut{0.5f, 1.0f, 0.3f};
    EXPECT_EQ(sut.values[0], sut.Value());
    EXPECT_EQ(sut.values[1], sut.Value());
    EXPECT_EQ(sut.values[2], sut.Value());
    EXPECT_EQ(sut.values[0], sut.Value());
}
