using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	class UnitTests_MatrixBoard
	{
		[Test]
		public void TestMatrixBoard()
		{
			var sut = new MatrixBoard(new Vector2Int(3, 3));
			var lPiece = new MatrixPiece(new Vector2Int(3, 2));
			lPiece.SetCell(new Vector2Int(0, 0), true);
			lPiece.SetCell(new Vector2Int(1, 0), true);
			lPiece.SetCell(new Vector2Int(2, 0), true);
			lPiece.SetCell(new Vector2Int(2, 1), true);

			var squarePiece = new MatrixPiece(new Vector2Int(2, 2));
			squarePiece.Fill();
			Assert.AreEqual(true, squarePiece.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(true, squarePiece.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(true, squarePiece.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(true, squarePiece.CellAt(new Vector2Int(1, 1)));

			Assert.AreEqual(true, sut.PutPiece(lPiece, new Vector2Int(0, 0)));
			Assert.AreEqual(false, sut.PutPiece(squarePiece, new Vector2Int(0, 0)));
			Assert.AreEqual(false, sut.PutPiece(squarePiece, new Vector2Int(1, 0)));
			Assert.AreEqual(false, sut.PutPiece(squarePiece, new Vector2Int(1, 1)));
			Assert.AreEqual(true, sut.PutPiece(squarePiece, new Vector2Int(0, 1)));

			Assert.AreEqual(lPiece, sut.PieceAt(new Vector2Int(0, 0)));
			Assert.AreEqual(lPiece, sut.PieceAt(new Vector2Int(1, 0)));
			Assert.AreEqual(lPiece, sut.PieceAt(new Vector2Int(2, 0)));
			Assert.AreEqual(lPiece, sut.PieceAt(new Vector2Int(2, 1)));

			Assert.AreEqual(squarePiece, sut.PieceAt(new Vector2Int(0, 1)));
			Assert.AreEqual(squarePiece, sut.PieceAt(new Vector2Int(1, 1)));
			Assert.AreEqual(squarePiece, sut.PieceAt(new Vector2Int(0, 2)));
			Assert.AreEqual(squarePiece, sut.PieceAt(new Vector2Int(1, 2)));

			Assert.AreEqual(new Vector2Int(0, 1), squarePiece.origin);
			Assert.AreEqual(new Vector2Int(0, 0), lPiece.origin);

			Assert.AreEqual(new Vector2Int(0, 0), sut.CellAt(new Vector2Int(0, 0)).pieceLocation);
			Assert.AreEqual(new Vector2Int(1, 0), sut.CellAt(new Vector2Int(1, 0)).pieceLocation);
			Assert.AreEqual(new Vector2Int(2, 0), sut.CellAt(new Vector2Int(2, 0)).pieceLocation);

			Assert.AreEqual(new Vector2Int(0, 0), sut.CellAt(new Vector2Int(0, 1)).pieceLocation);
			Assert.AreEqual(new Vector2Int(1, 0), sut.CellAt(new Vector2Int(1, 1)).pieceLocation);

			Assert.IsTrue(sut.IsCellBlockedAt(new Vector2Int(0, 0)));
			Assert.IsFalse(sut.IsCellBlockedAt(new Vector2Int(2, 2)));

			Assert.IsTrue(sut.IsPieceBlockedAt(new Vector2Int(0, 1), squarePiece));

			var excludeList = new HashSet<MatrixPiece>();
			excludeList.Add(squarePiece);
			Assert.IsFalse(sut.IsPieceBlockedAt(new Vector2Int(0, 1), squarePiece, excludeList));
			Assert.IsTrue(sut.IsPieceBlockedAt(new Vector2Int(1, 1), squarePiece, excludeList));

			sut.RemovePiece(squarePiece);
			Assert.IsFalse(sut.IsPieceBlockedAt(new Vector2Int(0, 1), squarePiece));

			Assert.AreEqual(null, sut.CellAt(new Vector2Int(0, 1)).piece);
			Assert.AreEqual(null, sut.CellAt(new Vector2Int(1, 1)).piece);
			Assert.AreEqual(null, sut.CellAt(new Vector2Int(0, 2)).piece);
			Assert.AreEqual(null, sut.CellAt(new Vector2Int(1, 2)).piece);
		}
	}
}
