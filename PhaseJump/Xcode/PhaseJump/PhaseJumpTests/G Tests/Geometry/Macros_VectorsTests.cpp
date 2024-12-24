#include "gtest/gtest.h"
#include "Macros_Vectors.h"
#include "Vector2.h"
#include "Vector3.h"

using namespace PJ;
using namespace std;

namespace PolygonTests {
}

using namespace PolygonTests;

TEST(Macros_Vectors, Operators)
{
    Vector2 sut;
    EXPECT_EQ(Vector2(0, 0), sut);
    
    sut = sut + Vector2(1, 2);
    EXPECT_EQ(Vector2(1, 2), sut);
    
    sut += Vector2(1, 2);
    EXPECT_EQ(Vector2(2, 4), sut);
    
    sut = sut - Vector2(1, 2);
    EXPECT_EQ(Vector2(1, 2), sut);
    
    sut -= Vector2(1, 2);
    EXPECT_EQ(Vector2(0, 0), sut);
    
    sut = {2, 2};
    sut = sut * Vector2{2, 3};
    EXPECT_EQ(Vector2(4, 6), sut);
    
    sut *= Vector2{2, 3};
    EXPECT_EQ(Vector2(8, 18), sut);
    
    sut = sut * 2;
    EXPECT_EQ(Vector2(16, 36), sut);
    
    sut *= 2;
    EXPECT_EQ(Vector2(32, 72), sut);
    
    sut = sut / 2;
    EXPECT_EQ(Vector2(16, 36), sut);
    
    sut /= 2;
    EXPECT_EQ(Vector2(8, 18), sut);
    
    sut = sut / Vector2{ 2, 3 };
    EXPECT_EQ(Vector2(4, 6), sut);
    
    sut /= Vector2{2, 3};
    EXPECT_EQ(Vector2(2, 2), sut);
}

TEST(Macros_Vectors, Magnitude)
{
    EXPECT_EQ(1, Vector2(0, 1).Magnitude());
    EXPECT_EQ(1, Vector2(1, 0).Magnitude());
    EXPECT_EQ(1, Vector2(0, -1).Magnitude());
    EXPECT_EQ(1, Vector2(-1, 0).Magnitude());
    
    EXPECT_NEAR(sqrtf(8.0f), Vector2(2, 2).Magnitude(), .00001f);
    EXPECT_NEAR(sqrtf(8.0f), Vector2(-2, 2).Magnitude(), .00001f);
    EXPECT_NEAR(sqrtf(8.0f), Vector2(2, -2).Magnitude(), .00001f);
}

TEST(Macros_Vectors, DotProductPerpendicular)
{
    EXPECT_EQ(0, Vector2(0, 1).DotProduct(Vector2(1, 0)));
}

TEST(Macros_Vectors, DotProduct)
{
    // From: https://math.libretexts.org/Bookshelves/Calculus/Calculus_(OpenStax)/12%3A_Vectors_in_Space/12.03%3A_The_Dot_Product
    EXPECT_EQ(12, Vector3(3, 5, 2).DotProduct(Vector3(-1, 3, 0)));
    EXPECT_EQ(7, Vector3(2, 9, -1).DotProduct(Vector3(-3, 1, -4)));
}
