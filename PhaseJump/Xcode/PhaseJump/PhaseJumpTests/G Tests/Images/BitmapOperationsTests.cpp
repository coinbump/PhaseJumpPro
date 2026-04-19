#include "gtest/gtest.h"

#include "Bitmap.h"
#include "BitmapOperations.h"

using namespace std;
using namespace PJ;

TEST(BitmapOperations, FlipV)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);
    sut.SetPixelColor(Vector2Int(0, 1), Color::blue);
    sut.SetPixelColor(Vector2Int(1, 1), Color::white);

    auto flipVOperation = BitmapOperations::NewFlipVertical(UpdateOrientationType::None);
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

TEST(BitmapOperations, FlipH)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 1));

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::blue);

    auto flipHOperation = BitmapOperations::NewFlipHorizontal();
    flipHOperation->Run(sut);

    auto scanlineData = sut.ScanLineData(0);
    EXPECT_EQ(2, scanlineData.size());
    EXPECT_EQ(Color::blue, scanlineData[0]);
    EXPECT_EQ(Color::red, scanlineData[1]);
}

TEST(BitmapOperations, RotateRight)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);
    sut.SetPixelColor(Vector2Int(0, 1), Color::blue);
    sut.SetPixelColor(Vector2Int(1, 1), Color::white);

    auto operation = BitmapOperations::NewRotateRight();
    operation->Run(sut);

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    {
        auto scanlineData = sut.ScanLineData(0);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::blue, scanlineData[0]);
        EXPECT_EQ(Color::red, scanlineData[1]);
    }

    {
        auto scanlineData = sut.ScanLineData(1);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::white, scanlineData[0]);
        EXPECT_EQ(Color::green, scanlineData[1]);
    }
}

TEST(BitmapOperations, RotateLeft)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(2, 2));

    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);
    sut.SetPixelColor(Vector2Int(0, 1), Color::blue);
    sut.SetPixelColor(Vector2Int(1, 1), Color::white);

    auto operation = BitmapOperations::NewRotateLeft();
    operation->Run(sut);

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    {
        auto scanlineData = sut.ScanLineData(0);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::green, scanlineData[0]);
        EXPECT_EQ(Color::white, scanlineData[1]);
    }

    {
        auto scanlineData = sut.ScanLineData(1);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::red, scanlineData[0]);
        EXPECT_EQ(Color::blue, scanlineData[1]);
    }
}

TEST(BitmapOperations, RotateRight_NonSquare)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(3, 2));

    // R G B
    // W K Y
    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);
    sut.SetPixelColor(Vector2Int(2, 0), Color::blue);
    sut.SetPixelColor(Vector2Int(0, 1), Color::white);
    sut.SetPixelColor(Vector2Int(1, 1), Color::black);
    sut.SetPixelColor(Vector2Int(2, 1), Color::yellow);

    auto operation = BitmapOperations::NewRotateRight();
    operation->Run(sut);

    EXPECT_EQ(Vector2Int(2, 3), sut.Size());

    // Expected after rotate right:
    // W R
    // K G
    // Y B
    {
        auto scanlineData = sut.ScanLineData(0);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::white, scanlineData[0]);
        EXPECT_EQ(Color::red, scanlineData[1]);
    }

    {
        auto scanlineData = sut.ScanLineData(1);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::black, scanlineData[0]);
        EXPECT_EQ(Color::green, scanlineData[1]);
    }

    {
        auto scanlineData = sut.ScanLineData(2);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::yellow, scanlineData[0]);
        EXPECT_EQ(Color::blue, scanlineData[1]);
    }
}

TEST(BitmapOperations, Stamp_SmallerIntoLarger)
{
    // 4x3 destination with a known background (red), 2x2 stamp (green) placed at (1, 1).
    // Expected:
    //   R R R R
    //   R G G R
    //   R G G R
    RGBABitmap dest;
    dest.Resize(Vector2Int(4, 3));
    dest.Fill(Color::red);

    RGBABitmap stamp;
    stamp.Resize(Vector2Int(2, 2));
    stamp.Fill(Color::green);

    auto operation = BitmapOperations::NewStamp(stamp, Vector2Int(1, 1));
    operation->Run(dest);

    // Row 0 unchanged
    {
        auto line = dest.ScanLineData(0);
        EXPECT_EQ(Color::red, line[0]);
        EXPECT_EQ(Color::red, line[1]);
        EXPECT_EQ(Color::red, line[2]);
        EXPECT_EQ(Color::red, line[3]);
    }
    // Row 1: stamp occupies columns 1 and 2
    {
        auto line = dest.ScanLineData(1);
        EXPECT_EQ(Color::red, line[0]);
        EXPECT_EQ(Color::green, line[1]);
        EXPECT_EQ(Color::green, line[2]);
        EXPECT_EQ(Color::red, line[3]);
    }
    // Row 2: stamp occupies columns 1 and 2
    {
        auto line = dest.ScanLineData(2);
        EXPECT_EQ(Color::red, line[0]);
        EXPECT_EQ(Color::green, line[1]);
        EXPECT_EQ(Color::green, line[2]);
        EXPECT_EQ(Color::red, line[3]);
    }
}

TEST(BitmapOperations, Stamp_SmallerIntoLargerNearEdge)
{
    // 3x3 destination (red background), 2x2 stamp (green) placed at (2, 2)
    // so only the stamp's top-left pixel lands inside the destination.
    // Expected:
    //   R R R
    //   R R R
    //   R R G
    RGBABitmap dest;
    dest.Resize(Vector2Int(3, 3));
    dest.Fill(Color::red);

    RGBABitmap stamp;
    stamp.Resize(Vector2Int(2, 2));
    stamp.Fill(Color::green);

    auto operation = BitmapOperations::NewStamp(stamp, Vector2Int(2, 2));
    operation->Run(dest);

    // Only the bottom-right pixel should change
    EXPECT_EQ(Color::red, *dest[Vector2Int(0, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(2, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(0, 1)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 1)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(2, 1)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(0, 2)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 2)]);
    EXPECT_EQ(Color::green, *dest[Vector2Int(2, 2)]);
}

TEST(BitmapOperations, Stamp_NegativePosition)
{
    // Negative position: stamp's (-1, -1) puts just its bottom-right corner inside.
    // 3x3 destination (red), 2x2 stamp (green) at (-1, -1)
    // Expected:
    //   G R R
    //   R R R
    //   R R R
    RGBABitmap dest;
    dest.Resize(Vector2Int(3, 3));
    dest.Fill(Color::red);

    RGBABitmap stamp;
    stamp.Resize(Vector2Int(2, 2));
    stamp.Fill(Color::green);

    auto operation = BitmapOperations::NewStamp(stamp, Vector2Int(-1, -1));
    operation->Run(dest);

    EXPECT_EQ(Color::green, *dest[Vector2Int(0, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(2, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(0, 1)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 1)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(2, 1)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(0, 2)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 2)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(2, 2)]);
}

TEST(BitmapOperations, Stamp_LargerIntoSmaller)
{
    // 2x2 destination (red background), 4x4 stamp (green) placed at (-1, -1).
    // The stamp extends past every edge of the destination. Only a 2x2 region of the
    // stamp (starting at its own (1,1)) lands inside the destination, fully covering it.
    // Expected:
    //   G G
    //   G G
    RGBABitmap dest;
    dest.Resize(Vector2Int(2, 2));
    dest.Fill(Color::red);

    RGBABitmap stamp;
    stamp.Resize(Vector2Int(4, 4));
    stamp.Fill(Color::green);

    auto operation = BitmapOperations::NewStamp(stamp, Vector2Int(-1, -1));
    operation->Run(dest);

    EXPECT_EQ(Vector2Int(2, 2), dest.Size());
    EXPECT_EQ(Color::green, *dest[Vector2Int(0, 0)]);
    EXPECT_EQ(Color::green, *dest[Vector2Int(1, 0)]);
    EXPECT_EQ(Color::green, *dest[Vector2Int(0, 1)]);
    EXPECT_EQ(Color::green, *dest[Vector2Int(1, 1)]);
}

TEST(BitmapOperations, Stamp_OutOfBoundsIsNoOp)
{
    // Position entirely past the destination's far edge — nothing should change.
    RGBABitmap dest;
    dest.Resize(Vector2Int(2, 2));
    dest.Fill(Color::red);

    RGBABitmap stamp;
    stamp.Resize(Vector2Int(2, 2));
    stamp.Fill(Color::green);

    auto operation = BitmapOperations::NewStamp(stamp, Vector2Int(5, 5));
    operation->Run(dest);

    EXPECT_EQ(Color::red, *dest[Vector2Int(0, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 0)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(0, 1)]);
    EXPECT_EQ(Color::red, *dest[Vector2Int(1, 1)]);
}

TEST(BitmapOperations, RotateLeft_NonSquare)
{
    RGBABitmap sut;
    sut.Resize(Vector2Int(3, 2));

    // R G B
    // W K Y
    sut.SetPixelColor(Vector2Int(0, 0), Color::red);
    sut.SetPixelColor(Vector2Int(1, 0), Color::green);
    sut.SetPixelColor(Vector2Int(2, 0), Color::blue);
    sut.SetPixelColor(Vector2Int(0, 1), Color::white);
    sut.SetPixelColor(Vector2Int(1, 1), Color::black);
    sut.SetPixelColor(Vector2Int(2, 1), Color::yellow);

    auto operation = BitmapOperations::NewRotateLeft();
    operation->Run(sut);

    EXPECT_EQ(Vector2Int(2, 3), sut.Size());

    // Expected after rotate left:
    // B Y
    // G K
    // R W
    {
        auto scanlineData = sut.ScanLineData(0);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::blue, scanlineData[0]);
        EXPECT_EQ(Color::yellow, scanlineData[1]);
    }

    {
        auto scanlineData = sut.ScanLineData(1);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::green, scanlineData[0]);
        EXPECT_EQ(Color::black, scanlineData[1]);
    }

    {
        auto scanlineData = sut.ScanLineData(2);
        EXPECT_EQ(2, scanlineData.size());
        EXPECT_EQ(Color::red, scanlineData[0]);
        EXPECT_EQ(Color::white, scanlineData[1]);
    }
}

