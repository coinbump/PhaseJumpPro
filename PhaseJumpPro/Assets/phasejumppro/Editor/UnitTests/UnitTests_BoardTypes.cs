using System.Collections;
using System.Collections.Generic;
using NUnit.Framework;
using PJ;

namespace EnumExtension
{
	class UnitTests_HexBoardDirectionExtensions
	{
		[Test]
		public void UnitTests()
		{
			var direction = PJ.MapDirection.North;
			direction = direction.Opposite();
			Assert.AreEqual(PJ.MapDirection.South, direction);
		}
	}
}
