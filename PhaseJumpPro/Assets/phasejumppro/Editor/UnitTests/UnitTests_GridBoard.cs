using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
    class UnitTests_GridBoard
    {
        private class TestGridLayer : GridLayer<GridTile>
        {
            public Rect2Int lastCellsBlocked;
            public Rect2Int lastCellsUnblocked;

            public TestGridLayer(Vector2Int size)
                : base(size)
            {
            }

            // OVERRIDE:
            public override void OnCellsBlocked(Rect2Int blocked)
            {
                lastCellsBlocked = blocked;
                base.OnCellsBlocked(blocked);
            }
            public override void OnCellsUnblocked(Rect2Int blocked)
            {
                lastCellsUnblocked = blocked;
                base.OnCellsUnblocked(blocked);
            }
        }

        private class TestGridBoard : GridBoard<GridTile>
        {
            // OVERRIDE:
            public override GridLayer<GridTile> NewLayer(Vector2Int size)
            {
                return new TestGridLayer(size);
            }
        }

        private class TestGridTile : GridTile
        {
            public int deconstruct;

            public TestGridTile(int deconstruct, int width = 2, int height = 2)
            {
                this.size = new Vector2Int(width, height);
                this.deconstruct = deconstruct;
            }
        }

        [Test]
        public void TestGridBoard_Test()
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

            GridCell<GridTile> gridCell = gridBoard.CellAt(new Vector3Int(0, 0, 0));
            Assert.AreEqual(null, gridCell.tile);
            Assert.AreEqual(null, gridBoard.TileAt(new Vector3Int(0, 0, 0)));

            int deconstruct = 0;
            TestGridTile tile = new TestGridTile(deconstruct);
            Assert.IsFalse(gridBoard.PutTile(tile, new Vector3Int(20, 20, 0)));
            Assert.IsFalse(gridBoard.PutTile(tile, new Vector3Int(0, 0, 2)));
            Assert.IsTrue(gridBoard.PutTile(tile, new Vector3Int(0, 0, 0)));

            gridCell = gridBoard.CellAt(new Vector3Int(0, 0, 0));
            Assert.AreEqual(tile, gridCell.tile);
            Assert.AreEqual(tile, gridBoard.TileAt(new Vector3Int(0, 0, 0)));
            Assert.AreEqual(tile, gridBoard.TileAt(new Vector3Int(0, 1, 0)));
            Assert.AreEqual(tile, gridBoard.TileAt(new Vector3Int(1, 0, 0)));
            Assert.AreEqual(tile, gridBoard.TileAt(new Vector3Int(1, 1, 0)));
            Assert.AreEqual(null, gridBoard.TileAt(new Vector3Int(0, 0, 1)));
            Assert.AreEqual(null, gridBoard.TileAt(new Vector3Int(2, 0, 0)));
            Assert.AreEqual(null, gridBoard.TileAt(new Vector3Int(0, 2, 0)));

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

            Rect2Int destTileBounds = gridBoard.DestTileBounds(tile, new Vector3Int(0, 0, 0));
            Assert.AreEqual(0, destTileBounds.position.x);
            Assert.AreEqual(0, destTileBounds.position.y);
            Assert.AreEqual(1, destTileBounds.position.x + destTileBounds.size.x - 1);
            Assert.AreEqual(1, destTileBounds.position.y + destTileBounds.size.y - 1);

            gridBoard.RemoveTile(tile, 0); tile = null;
            Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(0, 0, 0)));
            Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(0, 1, 0)));
            Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(1, 0, 0)));
            Assert.IsFalse(gridBoard.IsCellBlocked(new Vector3Int(1, 1, 0)));
            Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(0, 0), new Vector2Int(1, 1)), 0));
            Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(1, 0), new Vector2Int(2, 1)), 0));
            Assert.IsFalse(gridBoard.IsBlocked(new Rect2Int(new Vector2Int(0, 1), new Vector2Int(1, 2)), 0));
        }

        [Test]
        public void TextGridBoard_Axial()
        {
            var board = new MatrixAxial();
            Assert.AreEqual(1, board.NextAxialIndex(0, AxialDirection.Right));
            Assert.AreEqual(2, board.NextAxialIndex(1, AxialDirection.Right));
            Assert.AreEqual(3, board.NextAxialIndex(2, AxialDirection.Right));
            Assert.AreEqual(4, board.NextAxialIndex(3, AxialDirection.Right));
            Assert.AreEqual(5, board.NextAxialIndex(4, AxialDirection.Right));
            Assert.AreEqual(0, board.NextAxialIndex(7, AxialDirection.Right));

            Assert.AreEqual(7, board.NextAxialIndex(0, AxialDirection.Left));
            Assert.AreEqual(0, board.NextAxialIndex(1, AxialDirection.Left));
            Assert.AreEqual(1, board.NextAxialIndex(2, AxialDirection.Left));
            Assert.AreEqual(2, board.NextAxialIndex(3, AxialDirection.Left));
            Assert.AreEqual(3, board.NextAxialIndex(4, AxialDirection.Left));
            Assert.AreEqual(4, board.NextAxialIndex(5, AxialDirection.Left));
        }

        [Test]
        public void TestGridBoard_FullEmpty()
        {
            TestGridBoard gridBoard = new TestGridBoard();

            gridBoard.Resize(new Vector3Int(2, 2, 1));

            Assert.IsTrue(gridBoard.IsRowEmpty(new Vector3Int(0, 0, 0)));
            Assert.IsTrue(gridBoard.IsColumnEmpty(new Vector3Int(0, 0, 0)));
            Assert.IsFalse(gridBoard.IsRowFull(new Vector3Int(0, 0, 0)));
            Assert.IsFalse(gridBoard.IsColumnFull(new Vector3Int(0, 0, 0)));

            int deconstruct = 0;
            gridBoard.PutTile(new TestGridTile(deconstruct, 1, 1), new Vector3Int(0, 0, 0));

            Assert.IsFalse(gridBoard.IsRowEmpty(new Vector3Int(0, 0, 0)));
            Assert.IsFalse(gridBoard.IsColumnEmpty(new Vector3Int(0, 0, 0)));
            Assert.IsFalse(gridBoard.IsRowFull(new Vector3Int(0, 0, 0)));
            Assert.IsFalse(gridBoard.IsColumnFull(new Vector3Int(0, 0, 0)));

            gridBoard.PutTile(new TestGridTile(deconstruct, 1, 1), new Vector3Int(0, 1, 0));
            gridBoard.PutTile(new TestGridTile(deconstruct, 1, 1), new Vector3Int(1, 0, 0));

            Assert.IsTrue(gridBoard.IsRowFull(new Vector3Int(0, 0, 0)));
            Assert.IsTrue(gridBoard.IsColumnFull(new Vector3Int(0, 0, 0)));
        }

        [Test]
        public void TestGridBoard_TilesCount()
        {
            TestGridBoard gridBoard = new TestGridBoard();

            gridBoard.Resize(new Vector3Int(2, 2, 1));

            Assert.AreEqual(0, gridBoard.ColumnTilesCount(new Vector3Int(0, 0, 0)));
            Assert.AreEqual(0, gridBoard.RowTilesCount(new Vector3Int(0, 0, 0)));

            int deconstruct = 0;
            gridBoard.PutTile(new TestGridTile(deconstruct, 1, 1), new Vector3Int(0, 0, 0));

            Assert.AreEqual(1, gridBoard.ColumnTilesCount(new Vector3Int(0, 0, 0)));
            Assert.AreEqual(1, gridBoard.RowTilesCount(new Vector3Int(0, 0, 0)));
            Assert.AreEqual(0, gridBoard.ColumnTilesCount(new Vector3Int(1, 1, 0)));

            gridBoard.PutTile(new TestGridTile(deconstruct, 1, 1), new Vector3Int(1, 0, 0));
            gridBoard.PutTile(new TestGridTile(deconstruct, 1, 1), new Vector3Int(0, 1, 0));

            Assert.AreEqual(2, gridBoard.ColumnTilesCount(new Vector3Int(0, 0, 0)));
            Assert.AreEqual(2, gridBoard.RowTilesCount(new Vector3Int(0, 0, 0)));
        }
    }
}
