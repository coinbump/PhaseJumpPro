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
			Assert.IsFalse(new LogicOrTransform().Transform(false, false));
			Assert.IsTrue(new LogicOrTransform().Transform(true, false));
			Assert.IsFalse(new LogAndTransform().Transform(true, false));
			Assert.IsTrue(new LogAndTransform().Transform(true, true));
			Assert.IsFalse(new LogicXorTransform().Transform(false, false));
			Assert.IsTrue(new LogicXorTransform().Transform(true, false));
			Assert.IsFalse(new LogicXorTransform().Transform(true, true));
		}
	}
}