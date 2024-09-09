#include "gtest/gtest.h"

#undef _DISABLE_MIRRORS_

#include "PropertyMirror.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace PropertyMirrorTests {
    struct ChildMirror {
        ENABLE_MIRRORS("child")
        MIRROR(float, floatValue)
    };

    struct MirrorTest {
        ENABLE_MIRRORS("test")
        MIRROR(int, intValue)
        MIRROR(String, stringValue)
        MIRROR_CHILD(ChildMirror, childValue)
        MIRROR_CHILD_PTR(SP<ChildMirror>, childPtr)
    };
}

using namespace PropertyMirrorTests;

TEST(PropertyMirror, Test)
{
    MirrorTest sut;
    EXPECT_EQ(0, sut.intValue);
    EXPECT_EQ("", sut.stringValue);
    ASSERT_EQ(4, sut._mirrors.value.size());
    EXPECT_EQ("intValue", sut._mirrors.value[0]->name);
    EXPECT_EQ("stringValue", sut._mirrors.value[1]->name);

    auto intMirror = As<PropertyMirror<int>>(sut._mirrors.value[0]);
    ASSERT_NE(nullptr, intMirror);

    *intMirror = 10;
    EXPECT_EQ(10, sut.intValue);
    EXPECT_EQ(10, (int)(*intMirror));

    auto childValueMirror = As<ChildPropertyMirror>(sut._mirrors.value[2]);
    ASSERT_NE(nullptr, childValueMirror);

    PropertyMirrors const& childMirrorsFromProperty = sut.childValue._mirrors;
    PropertyMirrors const& childMirrorsFromMirror = *childValueMirror->mirrorsFunc();
    EXPECT_EQ(&childMirrorsFromProperty, &childMirrorsFromMirror);
    ASSERT_EQ(1, childMirrorsFromMirror.Count());
    EXPECT_EQ("floatValue", childMirrorsFromMirror.value[0]->name);
}
