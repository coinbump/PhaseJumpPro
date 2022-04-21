using UnityEngine;
using System.Collections;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_Tags
	{
		[Test]
		public void UnitTests()
		{
			Func<string> newString = delegate() { return string.Empty; };

			var tags = new Tags();
			Assert.AreEqual(tags.SafeValue<int>("test-int"), 0);
			Assert.AreEqual(tags.SafeValue<string>("test-string", newString), "");

			tags.Add("test-string-add", "add");
			Assert.AreEqual(tags.SafeValue<string>("test-string-add", newString), "add");

            tags["foo"] = 10;
			Assert.AreEqual(tags.SafeValue<int>("foo"), 10);
		}
	}
}
