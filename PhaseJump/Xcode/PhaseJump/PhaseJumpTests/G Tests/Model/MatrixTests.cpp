#include "gtest/gtest.h"
#include "Matrix.h"

using namespace PJ;
using namespace std;

namespace MatrixTests {
    enum class TestEnum
    {
        Invalid,
        Test1,
        Test2
    };

    class TestMatrix : public Matrix<int>
    {
    public:
        using Base = Matrix<int>;
        
        TestMatrix(Vector2Int size)
        : Base(size)
        {
        }
        
        size_t RowsCount() const { return size.y; }
        size_t ColumnsCount() const { return size.x; }
    };
}

using namespace MatrixTests;

TEST(Matrix, Test_Matrix)
{
    TestMatrix sut(Vector2Int(5, 5));

    EXPECT_EQ(5, sut.Width());
    EXPECT_EQ(5, sut.Height());
    EXPECT_EQ(5, sut.RowsCount());
    EXPECT_EQ(5, sut.ColumnsCount());

    sut.ResizeStable(Vector2Int(4, 4));
    EXPECT_EQ(4, sut.Width());
    EXPECT_EQ(4, sut.Height());
    EXPECT_EQ(4, sut.RowsCount());
    EXPECT_EQ(4, sut.ColumnsCount());

    sut.ResizeStable(Vector2Int(6, 6));
    EXPECT_EQ(6, sut.Width());
    EXPECT_EQ(6, sut.Height());
    EXPECT_EQ(6, sut.RowsCount());
    EXPECT_EQ(6, sut.ColumnsCount());

    EXPECT_FALSE(sut.IsValidLocation(Vector2Int(-1, -1)));
    EXPECT_FALSE(sut.IsValidLocation(Vector2Int(6, 0)));
    EXPECT_FALSE(sut.IsValidLocation(Vector2Int(0, 6)));
    EXPECT_TRUE(sut.IsValidLocation(Vector2Int(0, 0)));
    EXPECT_TRUE(sut.IsValidLocation(Vector2Int(5, 5)));

    EXPECT_EQ(0, sut.CellAt(Vector2Int(0, 0)));
    sut.SetCell(Vector2Int(0, 0), 3);
    EXPECT_EQ(3, sut.CellAt(Vector2Int(0, 0)));
}

TEST(Matrix, Test_Rotate1x1)
{
    Matrix<int> sut(Vector2Int(1, 1));
    EXPECT_EQ(false, sut.CellAt(Vector2Int(0, 0)));
    sut.SetCell(Vector2Int(0, 0), true);
    EXPECT_EQ(true, sut.CellAt(Vector2Int(0, 0)));

    sut.Rotate(SomeMatrix::RotateDirection::Right);
    EXPECT_EQ(true, sut.CellAt(Vector2Int(0, 0)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(true, sut.CellAt(Vector2Int(0, 0)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(true, sut.CellAt(Vector2Int(0, 0)));
}

TEST(Matrix, Test_Rotate2x1)
{
    Matrix<int> sut(Vector2Int(2, 1));
    EXPECT_EQ(false, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(false, sut.CellAt(Vector2Int(1, 0)));
    sut.SetCell(Vector2Int(1, 0), true);
    EXPECT_EQ(Vector2Int(2, 1), sut.Size());
    EXPECT_EQ(false, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(true, sut.CellAt(Vector2Int(1, 0)));

    sut.Rotate(SomeMatrix::RotateDirection::Right);
    EXPECT_EQ(Vector2Int(1, 2), sut.Size());
    EXPECT_EQ(false, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(true, sut.CellAt(Vector2Int(0, 1)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(Vector2Int(2, 1), sut.Size());
    EXPECT_EQ(false, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(true, sut.CellAt(Vector2Int(1, 0)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(Vector2Int(1, 2), sut.Size());
    EXPECT_EQ(true, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(false, sut.CellAt(Vector2Int(0, 1)));
}

TEST(Matrix, Test_Rotate2x2)
{
    Matrix<int> sut(Vector2Int(2, 2));
    sut.SetCell(Vector2Int(0, 0), 1);
    sut.SetCell(Vector2Int(1, 0), 2);
    sut.SetCell(Vector2Int(0, 1), 3);
    sut.SetCell(Vector2Int(1, 1), 4);
    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    sut.Rotate(SomeMatrix::RotateDirection::Right);
    // Careful: do not use {} style no-name inits with google test, it will crash Xcode due to a rampant memory leak
    EXPECT_EQ(Vector2Int(2, 2), sut.Size());
    EXPECT_EQ(3, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(1, sut.CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(4, sut.CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(2, sut.CellAt(Vector2Int(1, 1)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(Vector2Int(2, 2), sut.Size());
    EXPECT_EQ(1, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(2, sut.CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(3, sut.CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(4, sut.CellAt(Vector2Int(1, 1)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(Vector2Int(2, 2), sut.Size());
    EXPECT_EQ(2, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(4, sut.CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(1, sut.CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(3, sut.CellAt(Vector2Int(1, 1)));
}

TEST(Matrix, Test_Rotate3x2)
{
    Matrix<int> sut(Vector2Int(3, 2));
    sut.SetCell(Vector2Int(0, 0), 1);
    sut.SetCell(Vector2Int(1, 0), 2);
    sut.SetCell(Vector2Int(2, 0), 3);
    sut.SetCell(Vector2Int(0, 1), 4);
    sut.SetCell(Vector2Int(1, 1), 5);
    sut.SetCell(Vector2Int(2, 1), 6);
    EXPECT_EQ(Vector2Int(3, 2), sut.Size());
    EXPECT_EQ(1, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(2, sut.CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(3, sut.CellAt(Vector2Int(2, 0)));
    EXPECT_EQ(4, sut.CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(5, sut.CellAt(Vector2Int(1, 1)));
    EXPECT_EQ(6, sut.CellAt(Vector2Int(2, 1)));

    sut.Rotate(SomeMatrix::RotateDirection::Right);
    EXPECT_EQ(Vector2Int(2, 3), sut.Size());
    EXPECT_EQ(4, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(1, sut.CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(5, sut.CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(2, sut.CellAt(Vector2Int(1, 1)));
    EXPECT_EQ(6, sut.CellAt(Vector2Int(0, 2)));
    EXPECT_EQ(3, sut.CellAt(Vector2Int(1, 2)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(Vector2Int(3, 2), sut.Size());
    EXPECT_EQ(1, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(2, sut.CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(3, sut.CellAt(Vector2Int(2, 0)));
    EXPECT_EQ(4, sut.CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(5, sut.CellAt(Vector2Int(1, 1)));
    EXPECT_EQ(6, sut.CellAt(Vector2Int(2, 1)));

    sut.Rotate(SomeMatrix::RotateDirection::Left);
    EXPECT_EQ(Vector2Int(2, 3), sut.Size());
    EXPECT_EQ(3, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(6, sut.CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(2, sut.CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(5, sut.CellAt(Vector2Int(1, 1)));
    EXPECT_EQ(1, sut.CellAt(Vector2Int(0, 2)));
    EXPECT_EQ(4, sut.CellAt(Vector2Int(1, 2)));
}

TEST(Matrix, TestResizeStableBiggerCopy)
{
    Matrix<int> sut(Vector2Int(0, 0));
    sut.ResizeFast(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    int color1 = 10;
    int color2 = 20;

    sut.SetCell(Vector2Int(0, 0), color1);
    sut.SetCell(Vector2Int(1, 1), color2);

    EXPECT_EQ(color1, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(color2, sut.CellAt(Vector2Int(1, 1)));

    sut.ResizeStable(Vector2Int(3, 3));

    EXPECT_EQ(Vector2Int(3, 3), sut.Size());

    EXPECT_EQ(color1, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(color2, sut.CellAt(Vector2Int(1, 1)));
}

TEST(Matrix, TestResizeStableSmallerCopy)
{
    Matrix<int> sut(Vector2Int(0, 0));
    sut.ResizeFast(Vector2Int(4, 4));

    EXPECT_EQ(Vector2Int(4, 4), sut.Size());

    int color1 = 10;
    int color2 = 20;

    sut.SetCell(Vector2Int(0, 0), color1);
    sut.SetCell(Vector2Int(1, 1), color2);

    EXPECT_EQ(color1, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(color2, sut.CellAt(Vector2Int(1, 1)));

    sut.ResizeStable(Vector2Int(2, 2));

    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    EXPECT_EQ(color1, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(color2, sut.CellAt(Vector2Int(1, 1)));
}

TEST(Matrix, SetAllCells)
{
    Matrix<int> sut(Vector2Int(2, 2));
    EXPECT_EQ(0, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(0, sut.CellAt(Vector2Int(1, 1)));

    sut.SetAllCells(5);

    EXPECT_EQ(5, sut.CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(5, sut.CellAt(Vector2Int(1, 1)));
}

TEST(Matrix, CellAtException)
{
    Matrix<int> sut(Vector2Int(2, 2));

    EXPECT_ANY_THROW(sut.CellAt(Vector2Int(-1, -1)));
    EXPECT_ANY_THROW(sut.CellAt(Vector2Int(2, 2)));
}
