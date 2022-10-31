using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Factory
	{
		private class TestObject
        {
			public string id = "id";
        }

		[Test]
		public void UnitTests()
		{
			var factory = new Factory<TestObject>(() => new TestObject());
			object testObject = factory.NewObject();
			Assert.IsTrue(testObject is TestObject);
			Assert.AreEqual(factory.New().id, "id");
		}
	}
}
