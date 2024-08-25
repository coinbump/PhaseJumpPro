#include "gtest/gtest.h"

#include "Result.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

namespace ResultTests {
}


using namespace ResultTests;

TEST(Result, TestResult_ForSuccess)
{
    auto sut = Result<int, String>(10);
    EXPECT_EQ(ResultType::Success, sut.Type());
    EXPECT_EQ(10, sut.SuccessValue().value());
}

TEST(Result, TestResult_ForFailure)
{
    auto sut = Result<int, String>("test");
    EXPECT_EQ(ResultType::Failure, sut.Type());
    EXPECT_EQ("test", sut.FailureValue().value());
}

TEST(Result, TestRequestInvalidType_ForFailureValue)
{
    auto sut = Result<int, String>(10);
    EXPECT_EQ(ResultType::Success, sut.Type());
    EXPECT_FALSE(sut.FailureValue());
}

TEST(Result, TestRequestInvalidType_ForSuccessValue)
{
    auto sut = Result<int, String>("");
    EXPECT_EQ(ResultType::Failure, sut.Type());
    EXPECT_FALSE(sut.SuccessValue());
}
