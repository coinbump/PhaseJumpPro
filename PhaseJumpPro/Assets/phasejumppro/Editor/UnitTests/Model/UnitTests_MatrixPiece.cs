using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	class UnitTests_MatrixPIece
	{
		[Test]
		public void TestMatrixPiece_ClearAndFill()
		{
			var sut = new MatrixPiece(new Vector2Int(1, 1));
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));

			sut.Fill();
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 0)));

			sut.Clear();
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));
		}

		[Test]
		public void TestMatrixPiece_BuildFromShape()
		{
			var sut = new MatrixPiece(new Vector2Int(1, 1));
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));

			var pieceShape = new List<string>();
			pieceShape.Add(" *");
			pieceShape.Add("*");

			sut.BuildFromShape(pieceShape);

			Assert.AreEqual(2, sut.Width);
			Assert.AreEqual(2, sut.Height);

			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(1, 1)));
		}
	}
}
