using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	class UnitTests_GridStorage
	{
		private class TestGridStorage : GenericGrid<int>
		{
			public TestGridStorage(Vector2Int size)
				: base(size)
			{

			}

			public int NumRows() { return rows.Count; }
			public int NumColumns() { return rows.Count > 0 ? rows[0].Count : 0; }
		}

		[Test]
		public void UnitTests()
		{
			var gridStorage = new TestGridStorage(new Vector2Int(5, 5));

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

			Assert.IsFalse(gridStorage.IsValidLoc(new Vector2Int(-1, -1)));
			Assert.IsFalse(gridStorage.IsValidLoc(new Vector2Int(6, 0)));
			Assert.IsFalse(gridStorage.IsValidLoc(new Vector2Int(0, 6)));
			Assert.IsTrue(gridStorage.IsValidLoc(new Vector2Int(0, 0)));
			Assert.IsTrue(gridStorage.IsValidLoc(new Vector2Int(5, 5)));

			Assert.AreEqual(0, gridStorage.GetCell(new Vector2Int(0, 0)));
			gridStorage.SetCell(new Vector2Int(0, 0), 3);
			Assert.AreEqual(3, gridStorage.GetCell(new Vector2Int(0, 0)));
		}
	}
}
