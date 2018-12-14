using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_ConceptDemo
	{
		private class TestConcept : Concept
		{

		}

		private class TestDemo : Demo
		{
			public string output;
			public string value;

			public TestDemo()
			{

			}

			public TestDemo(string value)
			{
				this.value = value;
			}

			protected void ShowDemo()
			{
				output += value;
			}

			public override void Go()
			{
				base.Go();
				output += "go";

				ShowDemo();
				Finish();
			}
		}

		[Test]
		public void UnitTests()
		{
			var testConcept = new TestConcept();
			var testDemo1 = new TestDemo("1");
			var testDemo2 = new TestDemo("2");

			testConcept.AddDemo(testDemo1);
			testConcept.AddDemo(testDemo2);

			testConcept.Go();
			Assert.AreEqual("go1", testDemo1.output);
			Assert.AreEqual("go2", testDemo2.output);
			Assert.IsTrue(testDemo1.IsFinished());
			Assert.IsTrue(testDemo2.IsFinished());
			Assert.IsTrue(testConcept.IsFinished());
		}
	}

}
