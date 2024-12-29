#include "gtest/gtest.h"

#include "Bitmap.h"
#include "BitmapOperations.h"

using namespace std;
using namespace PJ;

TEST(Bitmap, TestDataInit)
{
    Data data;
    data.ResizeBytes(4);

    uint32_t testValue = 0xEF1fAABB;
    *((uint32_t*)data.Pointer()) = testValue;

    RGBABitmap sut({.size = { 1, 1 }, .pixels = data.Pointer(), .pixelsDataSize = data.Size() });

    EXPECT_EQ(Vector2Int(1, 1), sut.Size());
    EXPECT_EQ(testValue, *((uint32_t*)sut[0]));
    EXPECT_EQ(testValue, *((uint32_t*)sut[Vector2Int(0, 0)]));
}

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
    EXPECT_EQ(0, sut.Data().size());
    EXPECT_EQ(0, sut.DataSize());
}

TEST(Bitmap, TestResizeBiggerCopy)
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

TEST(Bitmap, TestResizeSmallerCopy)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(4, 4));

    EXPECT_EQ(Vector2Int(4, 4), sut.Size());

    Color color1 = Color::red;
    Color color2 = Color::green;

    sut.SetPixelColor(Vector2Int(0, 0), color1);
    sut.SetPixelColor(Vector2Int(1, 1), color2);

    EXPECT_EQ(color1, sut.PixelColorAt(Vector2Int(0, 0)));
    EXPECT_EQ(color2, sut.PixelColorAt(Vector2Int(1, 1)));

    sut.Resize(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    EXPECT_EQ((RGBAColor)color1, sut.PixelColor32At(Vector2Int(0, 0)));
    EXPECT_EQ((RGBAColor)color2, sut.PixelColor32At(Vector2Int(1, 1)));
}

TEST(Bitmap, HScanlineDataValid)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);

    auto scanlineData = sut.ScanLineData(0);
    EXPECT_FALSE(scanlineData.empty());
    EXPECT_EQ(2, scanlineData.size());

    EXPECT_EQ(Color::red, scanlineData[0]);
    EXPECT_EQ(Color::green, scanlineData[1]);
}

TEST(Bitmap, HScanlineDataInvalid)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());
    EXPECT_TRUE(sut.ScanLineData(-1).empty());
    EXPECT_EQ(0, sut.ScanLineData(-1).size());
    EXPECT_TRUE(sut.ScanLineData(3).empty());
    EXPECT_EQ(0, sut.ScanLineData(3).size());
}

TEST(Bitmap, FlipV)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);
    sut.SetPixelColor(Vector2Int(0, 1), Color::blue);
    sut.SetPixelColor(Vector2Int(1, 1), Color::white);

    auto flipVOperation = BitmapOperations::FlipV();
    flipVOperation->Run(sut);
    
    {
        auto scanlineData = sut.ScanLineData(0);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::blue, scanlineData[0]);
        EXPECT_EQ(Color::white, scanlineData[1]);
    }

    {
        auto scanlineData = sut.ScanLineData(1);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::red, scanlineData[0]);
        EXPECT_EQ(Color::green, scanlineData[1]);
    }
}

TEST(Bitmap, AnyPixelsOnScanLine)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 1));

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);

    EXPECT_TRUE(sut.AnyPixelsOnScanLine(0, [](auto const& pixel) {
        return pixel == Color::red;
    }));
    EXPECT_FALSE(sut.AnyPixelsOnScanLine(0, [](auto const& pixel) {
        return pixel == Color::blue;
    }));
}

TEST(Bitmap, FirstPixelIndexOnScanLine)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 1));

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);

    EXPECT_EQ(0, sut.FirstPixelIndexOnScanLine(0, [](auto const& pixel) {
        return pixel == Color::red;
    }));
    EXPECT_EQ(1, sut.FirstPixelIndexOnScanLine(0, [](auto const& pixel) {
        return pixel == Color::green;
    }));
    EXPECT_EQ(std::nullopt, sut.FirstPixelIndexOnScanLine(0, [](auto const& pixel) {
        return pixel == Color::blue;
    }));
}
