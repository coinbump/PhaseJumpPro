#include "gtest/gtest.h"

#include "Bitmap.h"

using namespace std;
using namespace PJ;

TEST(Bitmap, TestResize)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());
    EXPECT_EQ(16, sut.DataSize());
}

TEST(Bitmap, TestResizeZero)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(0, 0));

    EXPECT_EQ(Vector2Int(0, 0), sut.Size());
    EXPECT_EQ(nullptr, sut.Data());
    EXPECT_EQ(0, sut.DataSize());
}

TEST(Bitmap, TestResizeCopy)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());
    EXPECT_EQ(16, sut.DataSize());

    Color color1 = Color::red;
    Color color2 = Color::green;

    sut.SetPixelColor(Vector2Int(0, 0), color1);
    sut.SetPixelColor(Vector2Int(1, 1), color2);

    EXPECT_EQ(color1, sut.PixelColorAt(Vector2Int(0, 0)));
    EXPECT_EQ(color2, sut.PixelColorAt(Vector2Int(1, 1)));

    sut.Resize(Vector2Int(3, 3));

    EXPECT_EQ(Vector2Int(3, 3), sut.Size());
    EXPECT_EQ(36, sut.DataSize());

    EXPECT_EQ((RGBAColor)color1, sut.PixelColor32At(Vector2Int(0, 0)));
    EXPECT_EQ((RGBAColor)color2, sut.PixelColor32At(Vector2Int(1, 1)));
}
