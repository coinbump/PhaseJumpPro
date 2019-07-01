using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{


	// TODO: port C++ Unit Tests to C#

	class TestGridLayer : GenericGridLayer<GridTile> {
		public Rect2Int lastCellsBlocked;
		public Rect2Int lastCellsUnblocked;

		public TestGridLayer(Vector2Int size)
			: base(size)
		{
		}

		// OVERRIDE:
		public override void EvtCellsBlocked(Rect2Int blocked)
		{
			lastCellsBlocked = blocked;
			base.EvtCellsBlocked(blocked);
		}
		public override void EvtCellsUnblocked(Rect2Int blocked)
		{
			lastCellsUnblocked = blocked;
			base.EvtCellsUnblocked(blocked);
		}
	}

	class TestGridBoard : GenericGridBoard<GridTile>
	{
		// OVERRIDE:
		public override GenericGridLayer<GridTile> NewLayer(Vector2Int size)
		{
			return new TestGridLayer(size);
		}

		public int deconstruct;
	}

	//class TestGridTile : PJ_GridTile {
	//public:
	//	int deconstruct;
	//new Vector2Int mOldLoc;
	//PJ_String mObject;

	//TestGridTile(int deconstruct)
	//:   PJ_GridTile(PJ_Vector2Int(2,2))
	//	{
	//		 deconstruct = 0;
	//deconstruct = deconstruct;
	//	}
	//	virtual ~TestGridTile()
	//{
	//	deconstruct = 1;
	//}

	//};

	//void TestGridBoard::FillWith1by1()
	//{
	//	for (int x = 0; x < this.Width(); x++)
	//	{
	//		for (int y = 0; y < this.Height(); y++)
	//		{
	//			TestGridTile tile = new TestGridTile(&deconstruct);
	//			tile.mSize = PJ_Vector2Int(1, 1);
	//			this.PutTile(tile, new Vector2Int(x, y));
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
	//	TestGridTile tile = new TestGridTile(&deconstruct);
	//	Assert.AreEqual(true, gridBoard.PutTile(tile, new Vector2Int(0, 0)));
	//	Assert.AreEqual(static_cast<TestBoardGrid>(gridBoard.mGrids[0].get()).lastCellsBlocked, Rect2Int(0, 0, 1, 1));
	//	gridBoard.RemoveTile(tile);
	//	Assert.AreEqual(static_cast<TestBoardGrid>(gridBoard.mGrids[0].get()).lastCellsUnblocked, Rect2Int(0, 0, 1, 1));

	//	tile = new TestGridTile(&deconstruct);
	//	Assert.AreEqual(true, gridBoard.PutTile(tile, new Vector2Int(1, 1)));
	//	gridBoard.RemoveAllTiles();
	//	Assert.AreEqual(static_cast<TestBoardGrid>(gridBoard.mGrids[0].get()).lastCellsUnblocked, Rect2Int(1, 1, 2, 2));

	//}

	//// TODO: add unit tests.
	//TEST(GenericGridBoard<GridTile>, SwapAndSlide)
	//{

	//	{
	//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
	//		gridBoard.Build(5, 5, 2);
	//		gridBoard.FillWith1by1();

	//		EXPECT_FALSE(gridBoard.SwapColumn(new Vector2Int(0, 0, 0), new Vector2Int(0, 0, 0)));
	//		EXPECT_FALSE(gridBoard.SwapColumn(new Vector2Int(0, 0, 0), new Vector2Int(2, 0, 1)));
	//		Assert.AreEqual(true, gridBoard.SwapColumn(new Vector2Int(0, 0, 0), new Vector2Int(2, 0, 0)));

	//		for (int y = 0; y < gridBoard.Height(); y++)
	//		{
	//			Assert.AreEqual(new Vector2Int(2, y), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, y))).mOldLoc);
	//			Assert.AreEqual(new Vector2Int(0, y), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, y))).mOldLoc);
	//		}
	//	}

	//	{
	//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
	//		gridBoard.Build(5, 5, 2);
	//		gridBoard.FillWith1by1();

	//		EXPECT_FALSE(gridBoard.SwapRow(new Vector2Int(0, 0, 0), new Vector2Int(0, 0, 0)));
	//		EXPECT_FALSE(gridBoard.SwapRow(new Vector2Int(0, 0, 0), new Vector2Int(0, 2, 1)));
	//		Assert.AreEqual(true, gridBoard.SwapRow(new Vector2Int(0, 0, 0), new Vector2Int(0, 2, 0)));

	//		for (int x = 0; x < gridBoard.Width(); x++)
	//		{
	//			Assert.AreEqual(new Vector2Int(x, 2), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(x, 0))).mOldLoc);
	//			Assert.AreEqual(new Vector2Int(x, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(x, 2))).mOldLoc);
	//		}
	//	}

	//	// SlideColumn: WRAP
	//	{
	//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
	//		gridBoard.Build(5, 5, 2);
	//		gridBoard.FillWith1by1();

	//		gridBoard.SlideColumn(new Vector2Int(0, 0, 0), 3, true);
	//		Assert.AreEqual(new Vector2Int(0, 2), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 3), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 1))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 4), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 2))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 3))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 1), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 4))).mOldLoc);
	//	}

	//	// SlideColumn: WRAP-NEGATIVE
	//	{
	//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
	//		gridBoard.Build(5, 5, 2);
	//		gridBoard.FillWith1by1();

	//		gridBoard.SlideColumn(new Vector2Int(0, 0, 0), -2, true);
	//		Assert.AreEqual(new Vector2Int(0, 2), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 3), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 1))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 4), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 2))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 3))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 1), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 4))).mOldLoc);
	//	}

	//	// SlideColumn: NO WRAP
	//	{
	//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
	//		gridBoard.Build(5, 5, 2);
	//		gridBoard.FillWith1by1();

	//		gridBoard.SlideColumn(new Vector2Int(0, 0, 0), 3, false);
	//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))));
	//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 1))));
	//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 2))));
	//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 3))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 1), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 4))).mOldLoc);
	//	}

	//	// SlideRow: WRAP
	//	{
	//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
	//		gridBoard.Build(5, 5, 2);
	//		gridBoard.FillWith1by1();

	//		gridBoard.SlideRow(new Vector2Int(0, 0, 0), 3, true);
	//		Assert.AreEqual(new Vector2Int(2, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(3, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(1, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(4, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(3, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(1, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(4, 0))).mOldLoc);
	//	}

	//	// SlideRow: WRAP-NEGATIVE
	//	{
	//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
	//		gridBoard.Build(5, 5, 2);
	//		gridBoard.FillWith1by1();

	//		gridBoard.SlideRow(new Vector2Int(0, 0, 0), -2, true);
	//		Assert.AreEqual(new Vector2Int(2, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(3, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(1, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(4, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(3, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(1, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(4, 0))).mOldLoc);
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
	//				TestGridTile tile = new TestGridTile(&deconstruct);
	//				tile.mSize = PJ_Vector2Int(1, 1);
	//				gridBoard.PutTile(tile, new Vector2Int(x, y));
	//				tile.mOldLoc = tile.mOrigin;
	//			}
	//		}

	//		gridBoard.SlideRow(new Vector2Int(0, 0, 0), 3, false);
	//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))));
	//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(1, 0))));
	//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, 0))));
	//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(3, 0))).mOldLoc);
	//		Assert.AreEqual(new Vector2Int(1, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(4, 0))).mOldLoc);
	//	}

	//	// TODO: test no wrap, slide row, slide row-noWRAP

	//}

	//TEST(GenericGridBoard<GridTile>, Unit_Tests)
	//{

	//	PTR(TestGridBoard)  gridBoard(new TestGridBoard(BoardDistro::Track));
	//	gridBoard.Build(20, 20, 2);
	//	Assert.AreEqual(2, gridBoard.Depth());
	//	Assert.AreEqual(20, gridBoard.Width());
	//	Assert.AreEqual(20, gridBoard.Height());
	//	EXPECT_FALSE(gridBoard.IsValidLoc(new Vector2Int(0, 0, 2)));
	//	EXPECT_FALSE(gridBoard.IsValidLoc(new Vector2Int(0, 0, -1)));
	//	EXPECT_FALSE(gridBoard.IsValidLoc(new Vector2Int(20, 20)));
	//	EXPECT_FALSE(gridBoard.IsValidLoc(new Vector2Int(-1, -1)));
	//	Assert.AreEqual(true, gridBoard.IsValidLoc(new Vector2Int(19, 19)));
	//	Assert.AreEqual(true, gridBoard.IsValidLoc(new Vector2Int(0, 0)));

	//	PJ_GridCell gridCell = gridBoard.GetCell(new Vector2Int(0, 0));
	//	Assert.AreEqual(null, gridCell.mTile);
	//	Assert.AreEqual(null, gridBoard.GetTile(new Vector2Int(0, 0)));

	//	int deconstruct;
	//	TestGridTile tile = new TestGridTile(&deconstruct);
	//	EXPECT_FALSE(gridBoard.PutTile(tile, new Vector2Int(20, 20)));
	//	EXPECT_FALSE(gridBoard.PutTile(tile, new Vector2Int(0, 0, 2)));
	//	Assert.AreEqual(true, gridBoard.PutTile(tile, new Vector2Int(0, 0)));

	//	gridCell = gridBoard.GetCell(new Vector2Int(0, 0));
	//	Assert.AreEqual(tile, gridCell.mTile);
	//	Assert.AreEqual(tile, gridBoard.GetTile(new Vector2Int(0, 0)));
	//	Assert.AreEqual(tile, gridBoard.GetTile(new Vector2Int(0, 1)));
	//	Assert.AreEqual(tile, gridBoard.GetTile(new Vector2Int(1, 0)));
	//	Assert.AreEqual(tile, gridBoard.GetTile(new Vector2Int(1, 1)));
	//	Assert.AreEqual(null, gridBoard.GetTile(new Vector2Int(0, 0, 1)));
	//	Assert.AreEqual(null, gridBoard.GetTile(new Vector2Int(2, 0)));
	//	Assert.AreEqual(null, gridBoard.GetTile(new Vector2Int(0, 2)));

	//	Assert.AreEqual(true, gridBoard.IsCellBlocked(new Vector2Int(-1, -1)));
	//	Assert.AreEqual(true, gridBoard.IsCellBlocked(new Vector2Int(0, 0)));
	//	Assert.AreEqual(true, gridBoard.IsCellBlocked(new Vector2Int(0, 1)));
	//	Assert.AreEqual(true, gridBoard.IsCellBlocked(new Vector2Int(1, 0)));
	//	Assert.AreEqual(true, gridBoard.IsCellBlocked(new Vector2Int(1, 1)));
	//	EXPECT_FALSE(gridBoard.IsCellBlocked(new Vector2Int(0, 0, 1)));
	//	EXPECT_FALSE(gridBoard.IsCellBlocked(new Vector2Int(0, 2)));
	//	EXPECT_FALSE(gridBoard.IsCellBlocked(new Vector2Int(2, 0)));

	//	Assert.AreEqual(true, gridBoard.IsBlocked(Rect2Int(0, 0, 1, 1), 0));
	//	Assert.AreEqual(true, gridBoard.IsBlocked(Rect2Int(1, 0, 2, 1), 0));
	//	Assert.AreEqual(true, gridBoard.IsBlocked(Rect2Int(0, 1, 1, 2), 0));
	//	EXPECT_FALSE(gridBoard.IsBlocked(Rect2Int(2, 0, 3, 1), 0));
	//	EXPECT_FALSE(gridBoard.IsBlocked(Rect2Int(0, 2, 1, 3), 0));

	//	Rect2Int destTileBounds = gridBoard.GetDestTileBounds(tile, new Vector2Int(0, 0));
	//	Assert.AreEqual(0, destTileBounds.left());
	//	Assert.AreEqual(0, destTileBounds.top());
	//	Assert.AreEqual(1, destTileBounds.right());
	//	Assert.AreEqual(1, destTileBounds.bottom());

	//	Assert.AreEqual(0, deconstruct);
	//	gridBoard.RemoveTile(tile); tile = null;
	//	Assert.AreEqual(1, deconstruct);
	//	EXPECT_FALSE(gridBoard.IsCellBlocked(new Vector2Int(0, 0)));
	//	EXPECT_FALSE(gridBoard.IsCellBlocked(new Vector2Int(0, 1)));
	//	EXPECT_FALSE(gridBoard.IsCellBlocked(new Vector2Int(1, 0)));
	//	EXPECT_FALSE(gridBoard.IsCellBlocked(new Vector2Int(1, 1)));
	//	EXPECT_FALSE(gridBoard.IsBlocked(Rect2Int(0, 0, 1, 1), 0));
	//	EXPECT_FALSE(gridBoard.IsBlocked(Rect2Int(1, 0, 2, 1), 0));
	//	EXPECT_FALSE(gridBoard.IsBlocked(Rect2Int(0, 1, 1, 2), 0));

	//	tile = new TestGridTile(&deconstruct);
	//	Assert.AreEqual(true, gridBoard.PutTile(tile, new Vector2Int(0, 0)));
	//	Assert.AreEqual(0, deconstruct);
	//	gridBoard.RemoveAllTiles(); tile = null;
	//	Assert.AreEqual(1, deconstruct);

	//}

	//// TODO: add unit tests for all axial methods.
	//TEST(GenericGridBoard<GridTile>, Test_Axial)
	//{

	//	PTR(GenericGridBoard<GridTile>) board(new GenericGridBoard<GridTile>(BoardDistro::Ignore));
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

	//#endif
}