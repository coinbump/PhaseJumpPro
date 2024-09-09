#include "gtest/gtest.h"

namespace CPPTests {
};

using namespace std;
using namespace CPPTests;

TEST(CPP, MutableLambda) {
    stringstream ss;
    std::function<void()> sut = [value = 0, &ss]() mutable {
        value++;
        ss << value;
    };

    sut();
    EXPECT_EQ("1", ss.str());

    sut();
    EXPECT_EQ("12", ss.str());
}
