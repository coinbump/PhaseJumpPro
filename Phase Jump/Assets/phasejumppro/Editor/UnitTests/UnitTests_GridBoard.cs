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
		}

		class TestGridTile : GridTile
		{
			public int deconstruct;

			public TestGridTile(int deconstruct)
			{
				this.size = new Vector2Int(2, 2);
				this.deconstruct = deconstruct;
			}
		}

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