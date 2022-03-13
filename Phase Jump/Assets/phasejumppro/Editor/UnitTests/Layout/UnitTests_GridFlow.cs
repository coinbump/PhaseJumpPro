using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_GridFlow
	{
		[Test]
		public void TestGridFlow()
		{
			var logicModelZero = new GridFlow.GridLogicModel(2, 0);

			Assert.AreEqual(logicModelZero.ColumnCount, 2);
			Assert.AreEqual(logicModelZero.RowCount, 0);

			var logicModelThree = new GridFlow.GridLogicModel(2, 3);

			Assert.AreEqual(logicModelThree.ColumnCount, 2);
			Assert.AreEqual(logicModelThree.RowCount, 2);
			Assert.AreEqual(logicModelThree.ColumnIndexAt(0), 0);
			Assert.AreEqual(logicModelThree.ColumnIndexAt(1), 1);
			Assert.AreEqual(logicModelThree.ColumnIndexAt(2), 0);
			Assert.AreEqual(logicModelThree.ColumnIndexAt(3), 1);

			Assert.AreEqual(logicModelThree.RowIndexAt(0), 0);
			Assert.AreEqual(logicModelThree.RowIndexAt(1), 0);
			Assert.AreEqual(logicModelThree.RowIndexAt(2), 1);
			Assert.AreEqual(logicModelThree.RowIndexAt(3), 1);

			var logicModelFour = new GridFlow.GridLogicModel(2, 4);

			Assert.AreEqual(logicModelFour.ColumnCount, 2);
			Assert.AreEqual(logicModelFour.RowCount, 2);
		}
	}
}
