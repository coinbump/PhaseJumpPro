#include "gtest/gtest.h"
#include "Vector3.h"
#include "GeoTransform.h"
#include <memory>

using namespace std;
using namespace PJ;

namespace Vector3Tests {
}

using namespace Vector3Tests;

TEST(Vector3, Init) {
    Vector3 sut(1, 2, 3);
    EXPECT_EQ(1, sut[0]);
    EXPECT_EQ(2, sut[1]);
    EXPECT_EQ(3, sut[2]);
}

TEST(Vector3, Init_Default) {
    Vector3 sut;
    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(0, sut[1]);
    EXPECT_EQ(0, sut[2]);
}
