#include "gtest/gtest.h"

#include "Result.h"
#include "_String.h"

using namespace std;
using namespace PJ;

namespace ResultTests {
}


using namespace ResultTests;

TEST(Result, TestResult_ForSuccess)
{
    auto sut = Result<int, String>(10);
    EXPECT_EQ(ResultType::Success, sut.Type());
    EXPECT_EQ(10, sut.SuccessValue());
}

TEST(Result, TestResult_ForFailure)
{
    auto sut = Result<int, String>("test");
    EXPECT_EQ(ResultType::Failure, sut.Type());
    EXPECT_EQ("test", sut.FailureValue());
}
