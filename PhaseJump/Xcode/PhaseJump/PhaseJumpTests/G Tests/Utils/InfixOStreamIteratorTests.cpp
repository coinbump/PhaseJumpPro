#include "gtest/gtest.h"

#include "InfixOStreamIterator.h"
#include "VectorList.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

TEST(InfixOStreamIterator, TestJoinStrings1) {
    vector<String> strings{String("a")};
    std::stringstream stream;
    std::copy(strings.begin(), strings.end(), InfixOStreamIterator<String>(stream, ","));
    auto sut = stream.str();

    EXPECT_EQ("a", sut);
}

TEST(InfixOStreamIterator, TestJoinStrings2) {
    vector<String> strings{String("a"), String("b")};
    std::stringstream stream;
    std::copy(strings.begin(), strings.end(), InfixOStreamIterator<String>(stream, ","));
    auto sut = stream.str();

    EXPECT_EQ("a,b", sut);
}

TEST(InfixOStreamIterator, TestJoinStrings3) {
    vector<String> strings{String("a"), String("b"), String("c")};
    std::stringstream stream;
    std::copy(strings.begin(), strings.end(), InfixOStreamIterator<String>(stream, ","));
    auto sut = stream.str();

    EXPECT_EQ("a,b,c", sut);
}

TEST(InfixOStreamIterator, TestJoinInt3) {
    vector<int> list{0, 1, 2};
    std::stringstream stream;
    std::copy(list.begin(), list.end(), InfixOStreamIterator<int>(stream, ","));
    auto sut = stream.str();

    EXPECT_EQ("0,1,2", sut);
}
