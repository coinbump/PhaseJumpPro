#include "gtest/gtest.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace UtilsTests {
    struct A {
    public:
        virtual ~A() {}
    };
    
    struct B : public A {

    };
}

using namespace UtilsTests;

TEST(Utils, Is)
{
    A a;
    B b;

    EXPECT_FALSE(Is<B>(&a));
    EXPECT_TRUE(Is<A>(a));
    EXPECT_TRUE(Is<A>(b));
}
