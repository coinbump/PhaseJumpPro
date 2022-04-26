using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	class UnitTests_Matrix
	{
		private class TestMatrixStorage : Matrix<int>
		{
			public TestMatrixStorage(Vector2Int size)
				: base(size)
			{

			}

			public int NumRows() { return rows.Count; }
			public int NumColumns() { return rows.Count > 0 ? rows[0].Count : 0; }
		}

		[Test]
		public void TestMatrix()
		{
			var gridStorage = new TestMatrixStorage(new Vector2Int(5, 5));

			Assert.AreEqual(5, gridStorage.Width);
			Assert.AreEqual(5, gridStorage.Height);
			Assert.AreEqual(5, gridStorage.NumRows(), "Num Rows Check");
			Assert.AreEqual(5, gridStorage.NumColumns(), "Num Columns Check");

			gridStorage.Resize(new Vector2Int(4, 4));
			Assert.AreEqual(4, gridStorage.Width);
			Assert.AreEqual(4, gridStorage.Height);
			Assert.AreEqual(4, gridStorage.NumRows(), "Num Rows Check");
			Assert.AreEqual(4, gridStorage.NumColumns(), "Num Columns Check");

			gridStorage.Resize(new Vector2Int(6, 6));
			Assert.AreEqual(6, gridStorage.Width);
			Assert.AreEqual(6, gridStorage.Height);
			Assert.AreEqual(6, gridStorage.NumRows(), "Num Rows Check");
			Assert.AreEqual(6, gridStorage.NumColumns(), "Num Columns Check");

			Assert.IsFalse(gridStorage.IsValidLocation(new Vector2Int(-1, -1)));
			Assert.IsFalse(gridStorage.IsValidLocation(new Vector2Int(6, 0)));
			Assert.IsFalse(gridStorage.IsValidLocation(new Vector2Int(0, 6)));
			Assert.IsTrue(gridStorage.IsValidLocation(new Vector2Int(0, 0)));
			Assert.IsTrue(gridStorage.IsValidLocation(new Vector2Int(5, 5)));

			Assert.AreEqual(0, gridStorage.CellAt(new Vector2Int(0, 0)));
			gridStorage.SetCell(new Vector2Int(0, 0), 3);
			Assert.AreEqual(3, gridStorage.CellAt(new Vector2Int(0, 0)));
		}

		[Test]
		public void TestMatrix_Rotate1x1()
        {
			var sut = new Matrix<bool>(new Vector2Int(1, 1));
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));
			sut.SetCell(new Vector2Int(0, 0), true);
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 0)));

			sut.Rotate(SomeMatrix.RotateDirection.Right);
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 0)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 0)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 0)));
		}

		[Test]
		public void TestMatrix_Rotate2x1()
		{
			var sut = new Matrix<bool>(new Vector2Int(2, 1));
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(1, 0)));
			sut.SetCell(new Vector2Int(1, 0), true);
			Assert.AreEqual(new Vector2Int(2, 1), sut.Size);
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(1, 0)));

			sut.Rotate(SomeMatrix.RotateDirection.Right);
			Assert.AreEqual(new Vector2Int(1, 2), sut.Size);
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 1)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(new Vector2Int(2, 1), sut.Size);
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(1, 0)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(new Vector2Int(1, 2), sut.Size);
			Assert.AreEqual(true, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(false, sut.CellAt(new Vector2Int(1, 0)));
		}

		[Test]
		public void TestMatrix_Rotate2x2()
		{
			var sut = new Matrix<int>(new Vector2Int(2, 2));
			sut.SetCell(new Vector2Int(0, 0), 1);
			sut.SetCell(new Vector2Int(1, 0), 2);
			sut.SetCell(new Vector2Int(0, 1), 3);
			sut.SetCell(new Vector2Int(1, 1), 4);
			Assert.AreEqual(new Vector2Int(2, 2), sut.Size);

			sut.Rotate(SomeMatrix.RotateDirection.Right);
			Assert.AreEqual(new Vector2Int(2, 2), sut.Size);
			Assert.AreEqual(3, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(1, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(4, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(2, sut.CellAt(new Vector2Int(1, 1)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(new Vector2Int(2, 2), sut.Size);
			Assert.AreEqual(1, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(2, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(3, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(4, sut.CellAt(new Vector2Int(1, 1)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(new Vector2Int(2, 2), sut.Size);
			Assert.AreEqual(2, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(4, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(1, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(3, sut.CellAt(new Vector2Int(1, 1)));
		}

		[Test]
		public void TestMatrix_Rotate3x2()
		{
			var sut = new Matrix<int>(new Vector2Int(3, 2));
			sut.SetCell(new Vector2Int(0, 0), 1);
			sut.SetCell(new Vector2Int(1, 0), 2);
			sut.SetCell(new Vector2Int(2, 0), 3);
			sut.SetCell(new Vector2Int(0, 1), 4);
			sut.SetCell(new Vector2Int(1, 1), 5);
			sut.SetCell(new Vector2Int(2, 1), 6);
			Assert.AreEqual(new Vector2Int(3, 2), sut.Size);
			Assert.AreEqual(1, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(2, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(3, sut.CellAt(new Vector2Int(2, 0)));
			Assert.AreEqual(4, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(5, sut.CellAt(new Vector2Int(1, 1)));
			Assert.AreEqual(6, sut.CellAt(new Vector2Int(2, 1)));

			sut.Rotate(SomeMatrix.RotateDirection.Right);
			Assert.AreEqual(new Vector2Int(2, 3), sut.Size);
			Assert.AreEqual(4, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(1, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(5, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(2, sut.CellAt(new Vector2Int(1, 1)));
			Assert.AreEqual(6, sut.CellAt(new Vector2Int(0, 2)));
			Assert.AreEqual(3, sut.CellAt(new Vector2Int(1, 2)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(new Vector2Int(3, 2), sut.Size);
			Assert.AreEqual(1, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(2, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(3, sut.CellAt(new Vector2Int(2, 0)));
			Assert.AreEqual(4, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(5, sut.CellAt(new Vector2Int(1, 1)));
			Assert.AreEqual(6, sut.CellAt(new Vector2Int(2, 1)));

			sut.Rotate(SomeMatrix.RotateDirection.Left);
			Assert.AreEqual(new Vector2Int(2, 3), sut.Size);
			Assert.AreEqual(3, sut.CellAt(new Vector2Int(0, 0)));
			Assert.AreEqual(6, sut.CellAt(new Vector2Int(1, 0)));
			Assert.AreEqual(2, sut.CellAt(new Vector2Int(0, 1)));
			Assert.AreEqual(5, sut.CellAt(new Vector2Int(1, 1)));
			Assert.AreEqual(1, sut.CellAt(new Vector2Int(0, 2)));
			Assert.AreEqual(4, sut.CellAt(new Vector2Int(1, 2)));
		}
	}
}
