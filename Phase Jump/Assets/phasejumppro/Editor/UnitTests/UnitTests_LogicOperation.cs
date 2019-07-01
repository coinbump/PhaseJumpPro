using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_LogicOperation
	{

		[Test]
		public void UnitTests()
		{
			Assert.IsFalse(new LogicOrOperation().Transform(false, false));
			Assert.IsTrue(new LogicOrOperation().Transform(true, false));
			Assert.IsFalse(new LogicAndOperation().Transform(true, false));
			Assert.IsTrue(new LogicAndOperation().Transform(true, true));
			Assert.IsFalse(new LogicXorOperation().Transform(false, false));
			Assert.IsTrue(new LogicXorOperation().Transform(true, false));
			Assert.IsFalse(new LogicXorOperation().Transform(true, true));
		}
	}
}