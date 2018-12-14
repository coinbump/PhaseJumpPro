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

			Assert.AreEqual(am.GetIntVal("a", 0), 10);
			Assert.AreEqual(am.GetInt64Val("a", 0), 10);
			Assert.AreEqual(am.GetUIntVal("a", 0), 10);
			Assert.AreEqual(am.GetStrVal("a", ""), "10");
			Assert.AreEqual(am.GetBoolVal("a", false), false);

			am.AddAttribute("f", "1.1");
			Assert.LessOrEqual(Math.Abs(am.GetFloatVal("f", 0) - 1.1f), .001f);

			am.AddAttribute("b", "t");
			Assert.AreEqual(am.GetBoolVal("b", false), true);
		}
	}

}
