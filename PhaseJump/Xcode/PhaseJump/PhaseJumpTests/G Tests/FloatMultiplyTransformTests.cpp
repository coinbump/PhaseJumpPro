#include "gtest/gtest.h"
#include "TransformFunc.h"
#include "Utils.h"

#include <memory>

using namespace PJ;

TEST(FloatMultiplyTransform, Suffix_Prefix) {
    auto sut = TransformFuncs::Multiply<float>(2.0f);
    EXPECT_EQ(6.0f, sut(3.0f));
}
