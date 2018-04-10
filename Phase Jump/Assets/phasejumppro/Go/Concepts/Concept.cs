using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests
 * CODE REVIEW: 4.9.18
 */
namespace PJ
{
	/// <summary>
	/// Concept. The Concept-Demo pattern is useful for tutorials
	/// An app has N concepts, each concept has N demos
	/// Implementation is app dependent
	/// EXAMPLE: Concept: Plant some flowers. Demos: { point to flower, explanation text, wait for user to interact }
	/// </summary>
	public class Concept : Core
	{
		private int demoIndex;

		public List<Demo> demos = new List<Demo>();

		protected enum State
		{
			Invalid,
			Ready,
			Finished
		};
		protected GenericStateMachine<State> state = new GenericStateMachine<State>();

		protected virtual bool CheckFinished()
		{
			if (0 == demos.Count) { return true; }

			foreach (Demo demo in demos)
			{
				if (!demo.IsFinished())
				{
					return false;
				}
			}
			return true;
		}

		public Concept()
		{
			SetStateMachine(state);
			state.SetState(State.Ready);
		}

		public virtual void EvtDemoFinished(Demo demo)
		{
			demoIndex = demos.IndexOf(demo) + 1;

			if (CheckFinished())
			{
				state.SetState(State.Finished);
				return;
			}

			if (demoIndex < demos.Count)
			{
				var newDemo = demos[demoIndex];
				newDemo.Go();
			}
		}

		public bool IsReady() { return state.state == State.Ready; }
		public bool IsFinished() { return state.state == State.Finished; }

		public void AddDemo(Demo demo)
		{
			demos.Add(demo);
			demo.concept = this;
		}

		public override void Go() {
			base.Go();

			if (demoIndex < demos.Count) {
				demos[demoIndex].Go();
			}
		}
	}

	public class UnitTests_ConceptDemo {
		class TestConcept : Concept {
			
		}

		class TestDemo : Demo {
			public string output;
			public string value;

			public TestDemo() {
				
			}

			public TestDemo(string value) {
				this.value = value;
			}

			protected void ShowDemo() {
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
		public void UnitTests() {
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
