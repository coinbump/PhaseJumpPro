#include "gtest/gtest.h"
#include "Types/FloatTransform.h"

#include <memory>

using namespace PJ;

TEST(FloatMultiplyTransform, Suffix_Prefix) {
    auto sut = std::make_shared<FloatMultiplyTransform>();
    sut->factor = 2.0f;
    EXPECT_EQ(6.0f, sut->Transform(3.0f));
}
