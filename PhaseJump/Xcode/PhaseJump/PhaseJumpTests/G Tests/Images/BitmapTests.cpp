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

TEST(Bitmap, Fill_SetsAllPixels)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(3, 2));

    // Seed with a different color so we can detect coverage
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 3; x++) {
            sut.SetPixelColor(Vector2Int(x, y), Color::red);
        }
    }

    sut.Fill(Color::green);

    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 3; x++) {
            EXPECT_EQ(Color::green, sut.PixelColorAt(Vector2Int(x, y)));
        }
    }
}

TEST(Bitmap, Fill_OverwritesPreviousFill)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    sut.Fill(Color::red);
    sut.Fill(Color::blue);

    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            EXPECT_EQ(Color::blue, sut.PixelColorAt(Vector2Int(x, y)));
        }
    }
}

TEST(Bitmap, Fill_OnEmptyBitmapIsNoOp)
{
    // Default-constructed bitmap has no pixels — Fill should safely do nothing.
    RGBABitmap sut;
    sut.Fill(Color::red);

    EXPECT_EQ(0u, sut.DataSize());
}

TEST(Bitmap, WidthAndHeight)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(5, 3));

    EXPECT_EQ(5, sut.Width());
    EXPECT_EQ(3, sut.Height());
}

TEST(Bitmap, HasAlpha_RGBA)
{
    RGBABitmap sut;
    EXPECT_TRUE(sut.HasAlpha());
}

TEST(Bitmap, HasAlpha_BGRA)
{
    BGRABitmap sut;
    EXPECT_TRUE(sut.HasAlpha());
}

TEST(Bitmap, BGRABitmap_Fill)
{
    // Exercise the second pixel format: fill should go through BGRA8888::FromColor.
    BGRABitmap sut;
    sut.Resize(Vector2Int(2, 2));
    sut.Fill(Color::red);

    // Read back via operator[] to get the BGRAColor directly.
    BGRAColor const* pixel = sut[Vector2Int(0, 0)];
    ASSERT_NE(nullptr, pixel);
    EXPECT_NEAR(1.0f, pixel->redFloat(), 0.01f);
    EXPECT_NEAR(0.0f, pixel->greenFloat(), 0.01f);
    EXPECT_NEAR(0.0f, pixel->blueFloat(), 0.01f);
}

TEST(Bitmap, Resize_None_DoesNotPreservePixels)
{
    // CopyType::None fast path: pixels should be cleared (zero-initialized) after resize.
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));
    sut.Fill(Color::red);

    sut.Resize(Vector2Int(3, 3), CopyType::None);

    EXPECT_EQ(Vector2Int(3, 3), sut.Size());
    // None means "don't carry over" — old pixel at (0,0) is no longer red.
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            auto pixel = sut.PixelColor32At(Vector2Int(x, y));
            EXPECT_EQ(0u, (uint32_t)pixel);
        }
    }
}

TEST(Bitmap, Resize_None_FromEmpty)
{
    // CopyType::None path also handles the empty -> sized case.
    RGBABitmap sut;
    sut.Resize(Vector2Int(4, 2), CopyType::None);

    EXPECT_EQ(Vector2Int(4, 2), sut.Size());
    EXPECT_EQ(32u, sut.DataSize()); // 4*2 pixels * 4 bytes
}

TEST(Bitmap, MoveConstructor_TransfersPixels)
{
    RGBABitmap a;
    a.Resize(Vector2Int(2, 2));
    a.Fill(Color::green);

    RGBABitmap b = std::move(a);

    EXPECT_EQ(Vector2Int(2, 2), b.Size());
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            EXPECT_EQ(Color::green, b.PixelColorAt(Vector2Int(x, y)));
        }
    }
}

TEST(Bitmap, MoveAssignment_TransfersPixels)
{
    RGBABitmap a;
    a.Resize(Vector2Int(3, 1));
    a.Fill(Color::blue);

    RGBABitmap b;
    b = std::move(a);

    EXPECT_EQ(Vector2Int(3, 1), b.Size());
    for (int x = 0; x < 3; x++) {
        EXPECT_EQ(Color::blue, b.PixelColorAt(Vector2Int(x, 0)));
    }
}

TEST(Bitmap, LocToIndex)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(4, 3));

    // Linear layout: index = y * width + x
    EXPECT_EQ(0, sut.LocToIndex(Vector2Int(0, 0)));
    EXPECT_EQ(3, sut.LocToIndex(Vector2Int(3, 0)));
    EXPECT_EQ(4, sut.LocToIndex(Vector2Int(0, 1)));
    EXPECT_EQ(11, sut.LocToIndex(Vector2Int(3, 2)));

    // Out of bounds
    EXPECT_EQ(std::nullopt, sut.LocToIndex(Vector2Int(-1, 0)));
    EXPECT_EQ(std::nullopt, sut.LocToIndex(Vector2Int(0, -1)));
    EXPECT_EQ(std::nullopt, sut.LocToIndex(Vector2Int(4, 0)));
    EXPECT_EQ(std::nullopt, sut.LocToIndex(Vector2Int(0, 3)));
}

TEST(Bitmap, PixelDataAt_ConstOverload)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(3, 1));
    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);
    sut.SetPixelColor(Vector2Int(2, 0), Color::blue);

    RGBABitmap const& constRef = sut;
    auto span = constRef.PixelDataAt(Vector2Int(0, 0), 3);

    ASSERT_EQ(3u, span.size());
    EXPECT_EQ((RGBAColor)Color::red, span[0]);
    EXPECT_EQ((RGBAColor)Color::green, span[1]);
    EXPECT_EQ((RGBAColor)Color::blue, span[2]);

    // Returned span is span<Pixel const> — compile-time assert it's read-only
    static_assert(std::is_const_v<std::remove_reference_t<decltype(span[0])>>);
}

TEST(Bitmap, PixelDataAt_ClipsCountToBuffer)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(4, 1));

    // Request 100 pixels starting at index 1 — should clip to 3 (buffer size 4 - 1)
    auto span = sut.PixelDataAt(Vector2Int(1, 0), 100);
    EXPECT_EQ(3u, span.size());
}

TEST(Bitmap, PixelDataAt_OutOfBoundsReturnsEmpty)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    auto span = sut.PixelDataAt(Vector2Int(10, 10), 1);
    EXPECT_TRUE(span.empty());
}

TEST(Bitmap, DataPtr_NonConstAndConst)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 1));

    // Non-const path returns writable pointer
    void* writable = sut.DataPtr();
    EXPECT_NE(nullptr, writable);

    // Const path returns read-only pointer with the same address
    RGBABitmap const& constRef = sut;
    void const* readOnly = constRef.DataPtr();
    EXPECT_EQ(writable, readOnly);
}

TEST(Bitmap, Config_TruncatesOnOversizedBuffer)
{
    // Caller supplies more bytes than the bitmap needs — memcpy should clamp.
    Data data;
    data.ResizeBytes(16); // more than 1 pixel needs
    uint32_t testValue = 0xDEADBEEF;
    *((uint32_t*)data.Pointer()) = testValue;

    RGBABitmap sut({ .size = { 1, 1 }, .pixels = data.Pointer(), .pixelsDataSize = data.Size() });

    EXPECT_EQ(Vector2Int(1, 1), sut.Size());
    EXPECT_EQ(testValue, *((uint32_t*)sut[0]));
}
