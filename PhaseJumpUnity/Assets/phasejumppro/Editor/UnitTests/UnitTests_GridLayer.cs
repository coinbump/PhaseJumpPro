using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
}

// TODO: port C++ Unit tests to C#
//class TestBoardGrid : public PJ_BoardGrid {
//public:
//	PJ_VecRect2Int mLastCellsBlocked;
//PJ_VecRect2Int mLastCellsUnblocked;

//TestBoardGrid(PJ_GridBoard* owner, int width, int height, BoardDistro distro)
//:   PJ_BoardGrid(owner, width, height, distro)
//{
//}

//// OVERRIDE:
//virtual void evtCellsBlocked(PJ_VecRect2Int const& blocked)
//{
//	mLastCellsBlocked = blocked;
//	evtCellsBlocked(blocked);
//}
//virtual void evtCellsUnblocked(PJ_VecRect2Int const& blocked)
//{
//	mLastCellsUnblocked = blocked;
//	evtCellsUnblocked(blocked);
//}

//};

//class TestGridBoard : public PJ_GridBoard {
//protected:
//	// OVERRIDE:
//	virtual PJ_BoardGrid* newBoardGrid(int width, int height, BoardDistro distro)
//{
//	return new TestBoardGrid(this, width, height, distro);
//}

//public:
//	int mDeconstruct;

//TestGridBoard(BoardDistro distro)
//:   PJ_GridBoard(distro)
//{
//}

//void FillWith1by1();
//};

//class TestGridTile : public PJ_GridTile {
//public:
//	int* mDeconstruct;
//Vector3Int mOldLoc;
//PJ_String mObject;

//TestGridTile(int* deconstruct)
//:   PJ_GridTile(PJ_Vector3Int(2,2))
//	{
//		* deconstruct = 0;
//mDeconstruct = deconstruct;
//	}
//	virtual ~TestGridTile()
//{
//	*mDeconstruct = 1;
//}

//};

//void TestGridBoard::FillWith1by1()
//{
//	for (int x = 0; x < this.Width(); x++)
//	{
//		for (int y = 0; y < this.Height(); y++)
//		{
//			TestGridTile* tile = new TestGridTile(&mDeconstruct);
//			tile.mSize = PJ_Vector3Int(1, 1);
//			this.PutTile(tile, Vector3Int(x, y));
//			tile.mOldLoc = tile.mOrigin;
//		}
//	}

//}

//TEST(PJ_BoardGrid, Distro)
//{

//	PTR(TestGridBoard)  gridBoard(new TestGridBoard(BoardDistro::Track));
//	gridBoard.Build(20, 20, 2);
//	Assert.AreEqual(2, gridBoard.Depth());
//	Assert.AreEqual(20, gridBoard.Width());
//	Assert.AreEqual(20, gridBoard.Height());

//	int deconstruct;
//	TestGridTile* tile = new TestGridTile(&deconstruct);
//	EXPECT_TRUE(gridBoard.PutTile(tile, Vector3Int(0, 0)));
//	Assert.AreEqual(static_cast<TestBoardGrid*>(gridBoard.mGrids[0].get()).mLastCellsBlocked, PJ_VecRect2Int(0, 0, 1, 1));
//	gridBoard.RemoveTile(tile);
//	Assert.AreEqual(static_cast<TestBoardGrid*>(gridBoard.mGrids[0].get()).mLastCellsUnblocked, PJ_VecRect2Int(0, 0, 1, 1));

//	tile = new TestGridTile(&deconstruct);
//	EXPECT_TRUE(gridBoard.PutTile(tile, Vector3Int(1, 1)));
//	gridBoard.RemoveAllTiles();
//	Assert.AreEqual(static_cast<TestBoardGrid*>(gridBoard.mGrids[0].get()).mLastCellsUnblocked, PJ_VecRect2Int(1, 1, 2, 2));

//}

//// TODO: add unit tests.
//TEST(PJ_GridBoard, SwapAndSlide)
//{

//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		gridBoard.FillWith1by1();

//		EXPECT_FALSE(gridBoard.SwapColumn(Vector3Int(0, 0, 0), Vector3Int(0, 0, 0)));
//		EXPECT_FALSE(gridBoard.SwapColumn(Vector3Int(0, 0, 0), Vector3Int(2, 0, 1)));
//		EXPECT_TRUE(gridBoard.SwapColumn(Vector3Int(0, 0, 0), Vector3Int(2, 0, 0)));

//		for (int y = 0; y < gridBoard.Height(); y++)
//		{
//			Assert.AreEqual(Vector3Int(2, y), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, y))).mOldLoc);
//			Assert.AreEqual(Vector3Int(0, y), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(2, y))).mOldLoc);
//		}
//	}

//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		gridBoard.FillWith1by1();

//		EXPECT_FALSE(gridBoard.SwapRow(Vector3Int(0, 0, 0), Vector3Int(0, 0, 0)));
//		EXPECT_FALSE(gridBoard.SwapRow(Vector3Int(0, 0, 0), Vector3Int(0, 2, 1)));
//		EXPECT_TRUE(gridBoard.SwapRow(Vector3Int(0, 0, 0), Vector3Int(0, 2, 0)));

//		for (int x = 0; x < gridBoard.Width(); x++)
//		{
//			Assert.AreEqual(Vector3Int(x, 2), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(x, 0))).mOldLoc);
//			Assert.AreEqual(Vector3Int(x, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(x, 2))).mOldLoc);
//		}
//	}

//	// SlideColumn: WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		gridBoard.FillWith1by1();

//		gridBoard.SlideColumn(Vector3Int(0, 0, 0), 3, true);
//		Assert.AreEqual(Vector3Int(0, 2), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 3), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 1))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 4), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 2))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 3))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 1), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 4))).mOldLoc);
//	}

//	// SlideColumn: WRAP-NEGATIVE
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		gridBoard.FillWith1by1();

//		gridBoard.SlideColumn(Vector3Int(0, 0, 0), -2, true);
//		Assert.AreEqual(Vector3Int(0, 2), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 3), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 1))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 4), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 2))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 3))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 1), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 4))).mOldLoc);
//	}

//	// SlideColumn: NO WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		gridBoard.FillWith1by1();

//		gridBoard.SlideColumn(Vector3Int(0, 0, 0), 3, false);
//		Assert.AreEqual(null, static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 0))));
//		Assert.AreEqual(null, static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 1))));
//		Assert.AreEqual(null, static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 2))));
//		Assert.AreEqual(Vector3Int(0, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 3))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 1), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 4))).mOldLoc);
//	}

//	// SlideRow: WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		gridBoard.FillWith1by1();

//		gridBoard.SlideRow(Vector3Int(0, 0, 0), 3, true);
//		Assert.AreEqual(Vector3Int(2, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(3, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(1, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(4, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(2, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(3, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(1, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(4, 0))).mOldLoc);
//	}

//	// SlideRow: WRAP-NEGATIVE
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		gridBoard.FillWith1by1();

//		gridBoard.SlideRow(Vector3Int(0, 0, 0), -2, true);
//		Assert.AreEqual(Vector3Int(2, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(3, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(1, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(4, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(2, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(0, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(3, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(1, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(4, 0))).mOldLoc);
//	}

//	// SlideRow: NO WRAP
//	{
//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
//		gridBoard.Build(5, 5, 2);
//		int deconstruct = 0;
//		for (int x = 0; x < gridBoard.Width(); x++)
//		{
//			for (int y = 0; y < gridBoard.Height(); y++)
//			{
//				TestGridTile* tile = new TestGridTile(&deconstruct);
//				tile.mSize = PJ_Vector3Int(1, 1);
//				gridBoard.PutTile(tile, Vector3Int(x, y));
//				tile.mOldLoc = tile.mOrigin;
//			}
//		}

//		gridBoard.SlideRow(Vector3Int(0, 0, 0), 3, false);
//		Assert.AreEqual(null, static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(0, 0))));
//		Assert.AreEqual(null, static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(1, 0))));
//		Assert.AreEqual(null, static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(2, 0))));
//		Assert.AreEqual(Vector3Int(0, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(3, 0))).mOldLoc);
//		Assert.AreEqual(Vector3Int(1, 0), static_cast<TestGridTile*>(gridBoard.GetTile(Vector3Int(4, 0))).mOldLoc);
//	}

//	// TODO: test no wrap, slide row, slide row-noWRAP

//}


//TEST(PJ_GridBoard, Unit_Tests)
//{

//	PTR(TestGridBoard)  gridBoard(new TestGridBoard(BoardDistro::Track));
//	gridBoard.Build(20, 20, 2);
//	Assert.AreEqual(2, gridBoard.Depth());
//	Assert.AreEqual(20, gridBoard.Width());
//	Assert.AreEqual(20, gridBoard.Height());
//	EXPECT_FALSE(gridBoard.IsValidLoc(Vector3Int(0, 0, 2)));
//	EXPECT_FALSE(gridBoard.IsValidLoc(Vector3Int(0, 0, -1)));
//	EXPECT_FALSE(gridBoard.IsValidLoc(Vector3Int(20, 20)));
//	EXPECT_FALSE(gridBoard.IsValidLoc(Vector3Int(-1, -1)));
//	EXPECT_TRUE(gridBoard.IsValidLoc(Vector3Int(19, 19)));
//	EXPECT_TRUE(gridBoard.IsValidLoc(Vector3Int(0, 0)));

//	GridCell* gridCell = gridBoard.GetCell(Vector3Int(0, 0));
//	Assert.AreEqual(null, gridCell.tile);
//	Assert.AreEqual(null, gridBoard.GetTile(Vector3Int(0, 0)));

//	int deconstruct;
//	TestGridTile* tile = new TestGridTile(&deconstruct);
//	EXPECT_FALSE(gridBoard.PutTile(tile, Vector3Int(20, 20)));
//	EXPECT_FALSE(gridBoard.PutTile(tile, Vector3Int(0, 0, 2)));
//	EXPECT_TRUE(gridBoard.PutTile(tile, Vector3Int(0, 0)));

//	gridCell = gridBoard.GetCell(Vector3Int(0, 0));
//	Assert.AreEqual(tile, gridCell.tile);
//	Assert.AreEqual(tile, gridBoard.GetTile(Vector3Int(0, 0)));
//	Assert.AreEqual(tile, gridBoard.GetTile(Vector3Int(0, 1)));
//	Assert.AreEqual(tile, gridBoard.GetTile(Vector3Int(1, 0)));
//	Assert.AreEqual(tile, gridBoard.GetTile(Vector3Int(1, 1)));
//	Assert.AreEqual(null, gridBoard.GetTile(Vector3Int(0, 0, 1)));
//	Assert.AreEqual(null, gridBoard.GetTile(Vector3Int(2, 0)));
//	Assert.AreEqual(null, gridBoard.GetTile(Vector3Int(0, 2)));

//	EXPECT_TRUE(gridBoard.IsCellBlocked(Vector3Int(-1, -1)));
//	EXPECT_TRUE(gridBoard.IsCellBlocked(Vector3Int(0, 0)));
//	EXPECT_TRUE(gridBoard.IsCellBlocked(Vector3Int(0, 1)));
//	EXPECT_TRUE(gridBoard.IsCellBlocked(Vector3Int(1, 0)));
//	EXPECT_TRUE(gridBoard.IsCellBlocked(Vector3Int(1, 1)));
//	EXPECT_FALSE(gridBoard.IsCellBlocked(Vector3Int(0, 0, 1)));
//	EXPECT_FALSE(gridBoard.IsCellBlocked(Vector3Int(0, 2)));
//	EXPECT_FALSE(gridBoard.IsCellBlocked(Vector3Int(2, 0)));

//	EXPECT_TRUE(gridBoard.IsBlocked(PJ_VecRect2Int(0, 0, 1, 1), 0));
//	EXPECT_TRUE(gridBoard.IsBlocked(PJ_VecRect2Int(1, 0, 2, 1), 0));
//	EXPECT_TRUE(gridBoard.IsBlocked(PJ_VecRect2Int(0, 1, 1, 2), 0));
//	EXPECT_FALSE(gridBoard.IsBlocked(PJ_VecRect2Int(2, 0, 3, 1), 0));
//	EXPECT_FALSE(gridBoard.IsBlocked(PJ_VecRect2Int(0, 2, 1, 3), 0));

//	PJ_VecRect2Int destTileBounds = gridBoard.GetDestTileBounds(tile, Vector3Int(0, 0));
//	Assert.AreEqual(0, destTileBounds.left());
//	Assert.AreEqual(0, destTileBounds.top());
//	Assert.AreEqual(1, destTileBounds.right());
//	Assert.AreEqual(1, destTileBounds.bottom());

//	Assert.AreEqual(0, deconstruct);
//	gridBoard.RemoveTile(tile); tile = null;
//	Assert.AreEqual(1, deconstruct);
//	EXPECT_FALSE(gridBoard.IsCellBlocked(Vector3Int(0, 0)));
//	EXPECT_FALSE(gridBoard.IsCellBlocked(Vector3Int(0, 1)));
//	EXPECT_FALSE(gridBoard.IsCellBlocked(Vector3Int(1, 0)));
//	EXPECT_FALSE(gridBoard.IsCellBlocked(Vector3Int(1, 1)));
//	EXPECT_FALSE(gridBoard.IsBlocked(PJ_VecRect2Int(0, 0, 1, 1), 0));
//	EXPECT_FALSE(gridBoard.IsBlocked(PJ_VecRect2Int(1, 0, 2, 1), 0));
//	EXPECT_FALSE(gridBoard.IsBlocked(PJ_VecRect2Int(0, 1, 1, 2), 0));

//	tile = new TestGridTile(&deconstruct);
//	EXPECT_TRUE(gridBoard.PutTile(tile, Vector3Int(0, 0)));
//	Assert.AreEqual(0, deconstruct);
//	gridBoard.RemoveAllTiles(); tile = null;
//	Assert.AreEqual(1, deconstruct);

//}

//// TODO: add unit tests for all axial methods.
//TEST(PJ_GridBoard, Test_Axial)
//{

//	PTR(PJ_GridBoard) board(new PJ_GridBoard(BoardDistro::Ignore));
//	Assert.AreEqual(1, board.GetNextAxialIndex(0, AxialDir::Right));
//	Assert.AreEqual(2, board.GetNextAxialIndex(1, AxialDir::Right));
//	Assert.AreEqual(3, board.GetNextAxialIndex(2, AxialDir::Right));
//	Assert.AreEqual(4, board.GetNextAxialIndex(3, AxialDir::Right));
//	Assert.AreEqual(5, board.GetNextAxialIndex(4, AxialDir::Right));
//	Assert.AreEqual(0, board.GetNextAxialIndex(7, AxialDir::Right));

//	Assert.AreEqual(7, board.GetNextAxialIndex(0, AxialDir::Left));
//	Assert.AreEqual(0, board.GetNextAxialIndex(1, AxialDir::Left));
//	Assert.AreEqual(1, board.GetNextAxialIndex(2, AxialDir::Left));
//	Assert.AreEqual(2, board.GetNextAxialIndex(3, AxialDir::Left));
//	Assert.AreEqual(3, board.GetNextAxialIndex(4, AxialDir::Left));
//	Assert.AreEqual(4, board.GetNextAxialIndex(5, AxialDir::Left));

//}