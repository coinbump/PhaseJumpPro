#include "gtest/gtest.h"
#include "Font.h"

using namespace PJ;
using namespace std;

namespace FontTests {
}

using namespace FontTests;

TEST(Font, CanDisplay)
{
    Font sut;
    sut.glyphs["a"] = Font::Glyph();
    sut.glyphs["1"] = Font::Glyph();

    EXPECT_TRUE(sut.CanDisplay("a1"));
    EXPECT_TRUE(sut.CanDisplay("11"));
    EXPECT_FALSE(sut.CanDisplay("ab"));

    // TODO: add support for whitespace/carriage returns
}
