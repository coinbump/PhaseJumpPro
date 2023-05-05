#include "gtest/gtest.h"
#include "FloatTransform.h"
#include "Macros.h"

#include <memory>

using namespace PJ;

TEST(FloatMultiplyTransform, Suffix_Prefix) {
    auto sut = MAKE<FloatMultiplyTransform>();
    sut->factor = 2.0f;
    EXPECT_EQ(6.0f, sut->Transform(3.0f));
}
