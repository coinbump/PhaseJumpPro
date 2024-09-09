// Keep these, but don't uncomment them until you test this
// Creating a #define inside a gtest confuses it and breaks the other tests
//#include "gtest/gtest.h"
//
//#define _DISABLE_MIRRORS_
//
//#include "PropertyMirror.h"
//#include "Utils.h"
//
//using namespace PJ;
//using namespace std;
//
//namespace PropertyMirrorTests {
//    struct MirrorTest {
//        ENABLE_MIRRORS("test")
//        MIRROR(int, intValue, "Int value")
//        MIRROR(String, stringValue, "String value")
//    };
//}
//
//using namespace PropertyMirrorTests;
//
//TEST(PropertyMirror_Disabled, Test)
//{
//    MirrorTest sut;
//    EXPECT_EQ(0, sut.intValue);
//    EXPECT_EQ("", sut.stringValue);
//}
