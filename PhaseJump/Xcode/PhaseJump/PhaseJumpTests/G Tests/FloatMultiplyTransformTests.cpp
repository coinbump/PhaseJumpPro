#include "gtest/gtest.h"
#include "Types/FloatTransform.h"

using namespace PJ;

TEST(FloatMultiplyTransform, Suffix_Prefix) {
    auto sut = new FloatMultiplyTransform();
    sut->factor = 2.0f;
    EXPECT_EQ(6.0f, sut->Transform(3.0f));
}
