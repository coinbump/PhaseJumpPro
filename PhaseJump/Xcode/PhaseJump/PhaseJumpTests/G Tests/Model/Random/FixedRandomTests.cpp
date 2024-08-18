#include "gtest/gtest.h"
#include "FixedRandom.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace FixedRandomTests {
}

using namespace FixedRandomTests;

TEST(FixedRandom, Test) {
    FixedRandom sut{0.5f, 1.0f, 0.3f};
    EXPECT_EQ(sut.values[0], sut.Value());
    EXPECT_EQ(sut.values[1], sut.Value());
    EXPECT_EQ(sut.values[2], sut.Value());
    EXPECT_EQ(sut.values[0], sut.Value());
}
