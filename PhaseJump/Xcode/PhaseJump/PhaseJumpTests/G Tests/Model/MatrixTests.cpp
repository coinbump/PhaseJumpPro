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

    class TestMatrixStorage : public Matrix<int>
    {
    public:
        using Base = Matrix<int>;
        
        TestMatrixStorage(Vector2Int size)
        : Base(size)
        {
        }
        
        size_t RowsCount() const { return tuples.size(); }
        size_t ColumnsCount() const { return tuples.size() > 0 ? tuples[0].size() : 0; }
    };
}

using namespace MatrixTests;

TEST(Matrix, Test_Matrix)
{
    TestMatrixStorage gridStorage(Vector2Int(5, 5));

    EXPECT_EQ(5, gridStorage.Width());
    EXPECT_EQ(5, gridStorage.Height());
    EXPECT_EQ(5, gridStorage.RowsCount());
    EXPECT_EQ(5, gridStorage.ColumnsCount());

    gridStorage.Resize(Vector2Int(4, 4));
    EXPECT_EQ(4, gridStorage.Width());
    EXPECT_EQ(4, gridStorage.Height());
    EXPECT_EQ(4, gridStorage.RowsCount());
    EXPECT_EQ(4, gridStorage.ColumnsCount());

    gridStorage.Resize(Vector2Int(6, 6));
    EXPECT_EQ(6, gridStorage.Width());
    EXPECT_EQ(6, gridStorage.Height());
    EXPECT_EQ(6, gridStorage.RowsCount());
    EXPECT_EQ(6, gridStorage.ColumnsCount());

    EXPECT_FALSE(gridStorage.IsValidLocation(Vector2Int(-1, -1)));
    EXPECT_FALSE(gridStorage.IsValidLocation(Vector2Int(6, 0)));
    EXPECT_FALSE(gridStorage.IsValidLocation(Vector2Int(0, 6)));
    EXPECT_TRUE(gridStorage.IsValidLocation(Vector2Int(0, 0)));
    EXPECT_TRUE(gridStorage.IsValidLocation(Vector2Int(5, 5)));

    EXPECT_EQ(0, gridStorage.CellAt(Vector2Int(0, 0)));
    gridStorage.SetCell(Vector2Int(0, 0), 3);
    EXPECT_EQ(3, gridStorage.CellAt(Vector2Int(0, 0)));
}

TEST(Matrix, Test_Rotate1x1)
{
    Matrix<bool> sut(Vector2Int(1, 1));
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
    Matrix<bool> sut(Vector2Int(2, 1));
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
    EXPECT_EQ(false, sut.CellAt(Vector2Int(1, 0)));
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
