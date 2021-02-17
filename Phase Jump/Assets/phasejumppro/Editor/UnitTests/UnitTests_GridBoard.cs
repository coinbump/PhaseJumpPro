using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	class UnitTests_GridBoard
	{

		// TODO: port C++ Unit Tests to C#

		class TestGridLayer : GridLayer<GridTile>
		{
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

		class TestGridBoard : GridBoard<GridTile>
		{
			// OVERRIDE:
			public override GridLayer<GridTile> NewLayer(Vector2Int size)
			{
				return new TestGridLayer(size);
			}

			public int deconstruct;
		}

		class TestGridTile : GridTile
		{
			public int deconstruct;
			public Vector3Int oldOrigin;
			public string value;

			public TestGridTile(int deconstruct)
			{
				this.size = new Vector2Int(2, 2);
				this.deconstruct = deconstruct;
			}
		}

		//void TestGridBoard::FillWith1by1()
		//{
		//	for (int x = 0; x < this.Width(); x++)
		//	{
		//		for (int y = 0; y < this.Height(); y++)
		//		{
		//			TestGridTile tile = new TestGridTile(&deconstruct);
		//			tile.mSize = PJ_Vector2Int(1, 1);
		//			this.PutTile(tile, new Vector2Int(x, y));
		//			tile.oldOrigin = tile.mOrigin;
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
		//	Assert.IsTrue(gridBoard.PutTile(tile, new Vector2Int(0, 0)));
		//	Assert.AreEqual(static_cast<TestBoardGrid>(gridBoard.mGrids[0].get()).lastCellsBlocked, Rect2Int(0, 0, 1, 1));
		//	gridBoard.RemoveTile(tile);
		//	Assert.AreEqual(static_cast<TestBoardGrid>(gridBoard.mGrids[0].get()).lastCellsUnblocked, Rect2Int(0, 0, 1, 1));

		//	tile = new TestGridTile(&deconstruct);
		//	Assert.IsTrue(gridBoard.PutTile(tile, new Vector2Int(1, 1)));
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

		//		Assert.IsFalse(gridBoard.SwapColumn(new Vector2Int(0, 0, 0), new Vector2Int(0, 0, 0)));
		//		Assert.IsFalse(gridBoard.SwapColumn(new Vector2Int(0, 0, 0), new Vector2Int(2, 0, 1)));
		//		Assert.IsTrue(gridBoard.SwapColumn(new Vector2Int(0, 0, 0), new Vector2Int(2, 0, 0)));

		//		for (int y = 0; y < gridBoard.Height(); y++)
		//		{
		//			Assert.AreEqual(new Vector2Int(2, y), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, y))).oldOrigin);
		//			Assert.AreEqual(new Vector2Int(0, y), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, y))).oldOrigin);
		//		}
		//	}

		//	{
		//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
		//		gridBoard.Build(5, 5, 2);
		//		gridBoard.FillWith1by1();

		//		Assert.IsFalse(gridBoard.SwapRow(new Vector2Int(0, 0, 0), new Vector2Int(0, 0, 0)));
		//		Assert.IsFalse(gridBoard.SwapRow(new Vector2Int(0, 0, 0), new Vector2Int(0, 2, 1)));
		//		Assert.IsTrue(gridBoard.SwapRow(new Vector2Int(0, 0, 0), new Vector2Int(0, 2, 0)));

		//		for (int x = 0; x < gridBoard.Width(); x++)
		//		{
		//			Assert.AreEqual(new Vector2Int(x, 2), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(x, 0))).oldOrigin);
		//			Assert.AreEqual(new Vector2Int(x, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(x, 2))).oldOrigin);
		//		}
		//	}

		//	// SlideColumn: WRAP
		//	{
		//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
		//		gridBoard.Build(5, 5, 2);
		//		gridBoard.FillWith1by1();

		//		gridBoard.SlideColumn(new Vector2Int(0, 0, 0), 3, true);
		//		Assert.AreEqual(new Vector2Int(0, 2), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 3), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 1))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 4), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 2))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 3))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 1), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 4))).oldOrigin);
		//	}

		//	// SlideColumn: WRAP-NEGATIVE
		//	{
		//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
		//		gridBoard.Build(5, 5, 2);
		//		gridBoard.FillWith1by1();

		//		gridBoard.SlideColumn(new Vector2Int(0, 0, 0), -2, true);
		//		Assert.AreEqual(new Vector2Int(0, 2), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 3), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 1))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 4), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 2))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 3))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 1), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 4))).oldOrigin);
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
		//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 3))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 1), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 4))).oldOrigin);
		//	}

		//	// SlideRow: WRAP
		//	{
		//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
		//		gridBoard.Build(5, 5, 2);
		//		gridBoard.FillWith1by1();

		//		gridBoard.SlideRow(new Vector2Int(0, 0, 0), 3, true);
		//		Assert.AreEqual(new Vector2Int(2, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(3, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(1, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(4, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(3, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(1, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(4, 0))).oldOrigin);
		//	}

		//	// SlideRow: WRAP-NEGATIVE
		//	{
		//		PTR(TestGridBoard) gridBoard(new TestGridBoard(BoardDistro::Ignore));
		//		gridBoard.Build(5, 5, 2);
		//		gridBoard.FillWith1by1();

		//		gridBoard.SlideRow(new Vector2Int(0, 0, 0), -2, true);
		//		Assert.AreEqual(new Vector2Int(2, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(3, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(1, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(4, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(3, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(1, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(4, 0))).oldOrigin);
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
		//				tile.oldOrigin = tile.mOrigin;
		//			}
		//		}

		//		gridBoard.SlideRow(new Vector2Int(0, 0, 0), 3, false);
		//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(0, 0))));
		//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(1, 0))));
		//		Assert.AreEqual(null, static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(2, 0))));
		//		Assert.AreEqual(new Vector2Int(0, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(3, 0))).oldOrigin);
		//		Assert.AreEqual(new Vector2Int(1, 0), static_cast<TestGridTile>(gridBoard.GetTile(new Vector2Int(4, 0))).oldOrigin);
		//	}

		//	// TODO: test no wrap, slide row, slide row-noWRAP

		//}

		[Test]
		public void UnitTests()
		{
			TestGridBoard gridBoard = new TestGridBoard();

			gridBoard.Resize(new Vector3Int(20, 20, 2));
			Assert.AreEqual(2, gridBoard.Depth);
			Assert.AreEqual(20, gridBoard.Width);
			Assert.AreEqual(20, gridBoard.Height);
			Assert.IsFalse(gridBoard.IsValidLoc(new Vector3Int(0, 0, 2)));
			Assert.IsFalse(gridBoard.IsValidLoc(new Vector3Int(0, 0, -1)));
			Assert.IsFalse(gridBoard.IsValidLoc(new Vector3Int(20, 20, 0)));
			Assert.IsFalse(gridBoard.IsValidLoc(new Vector3Int(-1, -1, 0)));
			Assert.IsTrue(gridBoard.IsValidLoc(new Vector3Int(19, 19, 1)));
			Assert.IsTrue(gridBoard.IsValidLoc(new Vector3Int(0, 0, 0)));

			GridCell<GridTile> gridCell = gridBoard.GetCell(new Vector3Int(0, 0, 0));
			Assert.AreEqual(null, gridCell.tile);
			Assert.AreEqual(null, gridBoard.GetTile(new Vector3Int(0, 0, 0)));

			int deconstruct = 0;
			TestGridTile tile = new TestGridTile(deconstruct);
			Assert.IsFalse(gridBoard.PutTile(tile, new Vector3Int(20, 20, 0)));
			Assert.IsFalse(gridBoard.PutTile(tile, new Vector3Int(0, 0, 2)));
			Assert.IsTrue(gridBoard.PutTile(tile, new Vector3Int(0, 0, 0)));

			gridCell = gridBoard.GetCell(new Vector3Int(0, 0, 0));
			Assert.AreEqual(tile, gridCell.tile);
			Assert.AreEqual(tile, gridBoard.GetTile(new Vector3Int(0, 0, 0)));
			Assert.AreEqual(tile, gridBoard.GetTile(new Vector3Int(0, 1, 0)));
			Assert.AreEqual(tile, gridBoard.GetTile(new Vector3Int(1, 0, 0)));
			Assert.AreEqual(tile, gridBoard.GetTile(new Vector3Int(1, 1, 0)));
			Assert.AreEqual(null, gridBoard.GetTile(new Vector3Int(0, 0, 1)));
			Assert.AreEqual(null, gridBoard.GetTile(new Vector3Int(2, 0, 0)));
			Assert.AreEqual(null, gridBoard.GetTile(new Vector3Int(0, 2, 0)));

			Assert.IsTrue(gridBoard.IsCellBlocked(new Vector3Int(-1, -1, 0)));
			Assert.IsTrue(gridBoard.IsCellBlocked(new Vector3Int(0, 0, 0)));
			Assert.IsTrue(gridBoard.IsCellBlocked(new Vector3Int(0, 1, 0)));
			Assert.IsTrue(gridBoard.IsCellBlocked(new Vector3Int(1, 0, 0)));
			Assert.IsTrue(gridBoard.IsCellBlocked(new Vector3Int(1, 1, 0)));
			Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(0, 0, 1)));
			Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(0, 2, 0)));
			Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(2, 0, 0)));

			Assert.IsTrue(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(0, 0), new Vector2Int(1, 2)), 0));
			Assert.IsTrue(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(1, 0), new Vector2Int(2, 1)), 0));
			Assert.IsTrue(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(0, 1), new Vector2Int(1, 1)), 0));
			Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(2, 0), new Vector2Int(1, 1)), 0));
			Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(0, 2), new Vector2Int(1, 1)), 0));

			Rect2Int destTileBounds = gridBoard.GetDestTileBounds(tile, new Vector3Int(0, 0, 0));
			Assert.AreEqual(0, destTileBounds.origin.x);
			Assert.AreEqual(0, destTileBounds.origin.y);
			Assert.AreEqual(1, destTileBounds.origin.x + destTileBounds.size.x - 1);
			Assert.AreEqual(1, destTileBounds.origin.y + destTileBounds.size.y - 1);

			gridBoard.RemoveTile(tile); tile = null;
			Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(0, 0, 0)));
			Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(0, 1, 0)));
			Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(1, 0, 0)));
			Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(1, 1, 0)));
			Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(0, 0), new Vector2Int(1, 1)), 0));
			Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(1, 0), new Vector2Int(2, 1)), 0));
			Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(0, 1), new Vector2Int(1, 2)), 0));
		}

		[Test]
		public void AxialUnitTests() {

			var board = new GridBoard<GridTile>();
			Assert.AreEqual(1, board.GetNextAxialIndex(0, AxialDirection.Right));
			Assert.AreEqual(2, board.GetNextAxialIndex(1, AxialDirection.Right));
			Assert.AreEqual(3, board.GetNextAxialIndex(2, AxialDirection.Right));
			Assert.AreEqual(4, board.GetNextAxialIndex(3, AxialDirection.Right));
			Assert.AreEqual(5, board.GetNextAxialIndex(4, AxialDirection.Right));
			Assert.AreEqual(0, board.GetNextAxialIndex(7, AxialDirection.Right));

			Assert.AreEqual(7, board.GetNextAxialIndex(0, AxialDirection.Left));
			Assert.AreEqual(0, board.GetNextAxialIndex(1, AxialDirection.Left));
			Assert.AreEqual(1, board.GetNextAxialIndex(2, AxialDirection.Left));
			Assert.AreEqual(2, board.GetNextAxialIndex(3, AxialDirection.Left));
			Assert.AreEqual(3, board.GetNextAxialIndex(4, AxialDirection.Left));
			Assert.AreEqual(4, board.GetNextAxialIndex(5, AxialDirection.Left));
		}
	}
}