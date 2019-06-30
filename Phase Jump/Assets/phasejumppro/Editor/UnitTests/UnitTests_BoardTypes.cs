using System.Collections;
using System.Collections.Generic;
using NUnit.Framework;

namespace EnumExtension
{
	class UnitTests_HexBoardDirectionExtensions
	{
		[Test]
		public void UnitTests()
		{
			var direction = HexBoardDirection.North;
			direction = direction.Opposite();
			Assert.AreEqual(HexBoardDirection.South, direction);
		}
	}
}
