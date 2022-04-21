using System;
using System.Collections.Generic;
using NUnit.Framework;

/*
 * RATING: 5 stars. Modern, with Unit Tests
 * CODE REVIEW: 4.1.18
 */
namespace PJ
{
	public class UnitTests_AttributeMap
	{
		[Test]
		public void UnitTests()
		{
			var am = new AttributeMap();
			am.AddAttribute("a", "10");

			Assert.AreEqual(am.IntValue("a", 0), 10);
			Assert.AreEqual(am.Int64Value("a", 0), 10);
			Assert.AreEqual(am.UIntValue("a", 0), 10);
			Assert.AreEqual(am.StringValue("a", ""), "10");
			Assert.AreEqual(am.BoolValue("a", false), false);

			am.AddAttribute("f", "1.1");
			Assert.LessOrEqual(Math.Abs(am.FloatValue("f", 0) - 1.1f), .001f);

			am.AddAttribute("b", "t");
			Assert.AreEqual(am.BoolValue("b", false), true);
		}
	}

}
