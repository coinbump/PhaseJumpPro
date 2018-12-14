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
			Assert.AreEqual(false, new LogicOrOperation().Transform(false, false));
			Assert.AreEqual(true, new LogicOrOperation().Transform(true, false));
			Assert.AreEqual(false, new LogicAndOperation().Transform(true, false));
			Assert.AreEqual(true, new LogicAndOperation().Transform(true, true));
			Assert.AreEqual(false, new LogicXorOperation().Transform(false, false));
			Assert.AreEqual(true, new LogicXorOperation().Transform(true, false));
			Assert.AreEqual(false, new LogicXorOperation().Transform(true, true));
		}
	}
}