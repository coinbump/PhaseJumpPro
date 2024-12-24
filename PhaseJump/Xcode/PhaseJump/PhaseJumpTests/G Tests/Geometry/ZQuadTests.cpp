#include "gtest/gtest.h"
#include "ZQuad.h"

using namespace PJ;
using namespace std;

namespace ZQuadTests {
    Vec3 zero{0, 0, 0};
    Vec3 one{1, 1, 1};
    Vec3 two{2, 2, 2};
    Vec3 three{3, 3, 3};
    Vec3 four{4, 4, 4};
}

using namespace ZQuadTests;

TEST(ZQuad, Data)
{
    ZQuad sut{{zero, one, two, three}};
    EXPECT_EQ(zero, *sut.data());
}

TEST(ZQuad, ModGet)
{
    ZQuad sut{{zero, one, two, three}};
    EXPECT_EQ(zero, sut.ModGet(0));
    EXPECT_EQ(three, sut.ModGet(3));
    EXPECT_EQ(zero, sut.ModGet(4));
}

TEST(ZQuad, FlipH)
{
    ZQuad sut{{zero, one, two, three}};
    ZQuad expected{{one, zero, three, two}};
    sut.FlipH();
    EXPECT_EQ(expected, sut);
}

TEST(ZQuad, FlipV)
{
    ZQuad sut{{zero, one, two, three}};
    ZQuad expected{{two, three, zero, one}};
    sut.FlipV();
    EXPECT_EQ(expected, sut);
}

TEST(ZQuad, Offset)
{
    ZQuad sut{{zero, one, two, three}};
    sut.Offset(one);
    
    EXPECT_EQ(one, sut.value[0]);
    EXPECT_EQ(two, sut.value[1]);
    EXPECT_EQ(three, sut.value[2]);
    EXPECT_EQ(four, sut.value[3]);
}

TEST(ZQuad, VertexIndexToCornerIndex)
{
    EXPECT_EQ(0, ZQuad::VertexIndexToCornerIndex(0));
    EXPECT_EQ(1, ZQuad::VertexIndexToCornerIndex(1));
    EXPECT_EQ(3, ZQuad::VertexIndexToCornerIndex(2));
    EXPECT_EQ(2, ZQuad::VertexIndexToCornerIndex(3));
}

TEST(ZQuad, CornerIndexToVertexIndex)
{
    EXPECT_EQ(0, ZQuad::CornerIndexToVertexIndex(0));
    EXPECT_EQ(1, ZQuad::CornerIndexToVertexIndex(1));
    EXPECT_EQ(3, ZQuad::CornerIndexToVertexIndex(2));
    EXPECT_EQ(2, ZQuad::CornerIndexToVertexIndex(3));
}

TEST(ZQuad, Turn)
{
    ZQuad first{{zero, one, two, three}};
    ZQuad sut{{zero, one, two, three}};
    ZQuad second{{two, zero, three, one}};
    
    EXPECT_EQ(first, sut);
    
    sut.Turn(1);
    EXPECT_EQ(second, sut);
    
    sut.Turn(-1);
    EXPECT_EQ(first, sut);
}
