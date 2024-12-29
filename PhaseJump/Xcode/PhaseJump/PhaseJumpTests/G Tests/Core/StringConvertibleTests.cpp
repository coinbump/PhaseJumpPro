#include "gtest/gtest.h"
#include "StringConvertible.h"
#include "SomeKeyUIEvent.h"

using namespace PJ;
using namespace std;

namespace StringConvertibleTests {
    class TestConvertible : public StringConvertible {
    public:
        String ToString() const { return "test"; }
    };
}

using namespace StringConvertibleTests;

TEST(StringConvertible, Test)
{
    TestConvertible test;
    EXPECT_EQ("test", test.ToString());
    
    std::stringstream ss;
    ss << test;
    EXPECT_EQ("test", ss.str());
}
