#include "gtest/gtest.h"
#include "MatrixBoard.h"

using namespace PJ;
using namespace std;

namespace MatrixBoardTests {
}

using namespace MatrixBoardTests;

TEST(MatrixBoard, Test)
{
    MatrixBoard sut({3, 3});
    auto lPiece = MAKE<MatrixPiece>(Vector2Int(3, 2));
    lPiece->BuildFromShape({"***", "__*"});

    auto squarePiece = MAKE<MatrixPiece>(Vector2Int(2, 2));
    squarePiece->Fill();
    EXPECT_EQ(true, squarePiece->CellAt(Vector2Int(0, 0)));
    EXPECT_EQ(true, squarePiece->CellAt(Vector2Int(1, 0)));
    EXPECT_EQ(true, squarePiece->CellAt(Vector2Int(0, 1)));
    EXPECT_EQ(true, squarePiece->CellAt(Vector2Int(1, 1)));

    EXPECT_EQ(true, sut.PutPiece(lPiece, Vector2Int(0, 0)));
    EXPECT_EQ(false, sut.PutPiece(squarePiece, Vector2Int(0, 0)));
    EXPECT_EQ(false, sut.PutPiece(squarePiece, Vector2Int(1, 0)));
    EXPECT_EQ(false, sut.PutPiece(squarePiece, Vector2Int(1, 1)));
    EXPECT_EQ(true, sut.PutPiece(squarePiece, Vector2Int(0, 1)));

    EXPECT_EQ(lPiece, sut.PieceAt(Vector2Int(0, 0)));
    EXPECT_EQ(lPiece, sut.PieceAt(Vector2Int(1, 0)));
    EXPECT_EQ(lPiece, sut.PieceAt(Vector2Int(2, 0)));
    EXPECT_EQ(lPiece, sut.PieceAt(Vector2Int(2, 1)));

    EXPECT_EQ(squarePiece, sut.PieceAt(Vector2Int(0, 1)));
    EXPECT_EQ(squarePiece, sut.PieceAt(Vector2Int(1, 1)));
    EXPECT_EQ(squarePiece, sut.PieceAt(Vector2Int(0, 2)));
    EXPECT_EQ(squarePiece, sut.PieceAt(Vector2Int(1, 2)));

    EXPECT_EQ(Vector2Int(0, 1), squarePiece->origin);
    EXPECT_EQ(Vector2Int(0, 0), lPiece->origin);

    EXPECT_EQ(Vector2Int(0, 0), sut.CellAt(Vector2Int(0, 0)).pieceLoc);
    EXPECT_EQ(Vector2Int(1, 0), sut.CellAt(Vector2Int(1, 0)).pieceLoc);
    EXPECT_EQ(Vector2Int(2, 0), sut.CellAt(Vector2Int(2, 0)).pieceLoc);

    EXPECT_EQ(Vector2Int(0, 0), sut.CellAt(Vector2Int(0, 1)).pieceLoc);
    EXPECT_EQ(Vector2Int(1, 0), sut.CellAt(Vector2Int(1, 1)).pieceLoc);

    EXPECT_TRUE(sut.IsCellOccupied(Vector2Int(0, 0)));
    EXPECT_FALSE(sut.IsCellOccupied(Vector2Int(2, 2)));

    EXPECT_TRUE(sut.IsPieceBlockedAt(Vector2Int(0, 1), *squarePiece));

    UnorderedSet<SP<MatrixPiece>> excludeList;
    excludeList.insert(squarePiece);
    EXPECT_FALSE(sut.IsPieceBlockedAtExclude(Vector2Int(0, 1), *squarePiece, excludeList));
    EXPECT_TRUE(sut.IsPieceBlockedAtExclude(Vector2Int(1, 1), *squarePiece, excludeList));

    sut.Remove(squarePiece);
    EXPECT_FALSE(sut.IsPieceBlockedAt(Vector2Int(0, 1), *squarePiece));

    EXPECT_EQ(nullptr, sut.CellAt(Vector2Int(0, 1)).piece);
    EXPECT_EQ(nullptr, sut.CellAt(Vector2Int(1, 1)).piece);
    EXPECT_EQ(nullptr, sut.CellAt(Vector2Int(0, 2)).piece);
    EXPECT_EQ(nullptr, sut.CellAt(Vector2Int(1, 2)).piece);
}
