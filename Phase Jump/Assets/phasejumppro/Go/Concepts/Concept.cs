using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
			state.State = State.Ready;
		}

		public virtual void EvtDemoFinished(Demo demo)
		{
			demoIndex = demos.IndexOf(demo) + 1;

			if (CheckFinished())
			{
				state.State = State.Finished;
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
}
