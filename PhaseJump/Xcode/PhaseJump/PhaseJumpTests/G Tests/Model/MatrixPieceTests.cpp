#include "gtest/gtest.h"
#include "MatrixPiece.h"

using namespace PJ;
using namespace std;

namespace MatrixPieceTests {
}

using namespace MatrixPieceTests;

TEST(MatrixPiece, BuildFromShape)
{
    MatrixPiece sut({1, 1});
    EXPECT_EQ(Vector2Int(1, 1), sut.Size());

    VectorList<String> shape{" *", "* "};
    sut.BuildFromShape(shape);
    EXPECT_EQ(Vector2Int(2, 2), sut.Size());

    EXPECT_FALSE(sut.Matrix().CellAt({0, 0}));
    EXPECT_TRUE(sut.Matrix().CellAt({1, 0}));
    EXPECT_TRUE(sut.Matrix().CellAt({0, 1}));
    EXPECT_FALSE(sut.Matrix().CellAt({1, 1}));
}
