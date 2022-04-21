using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ {

	/// <summary>
	/// Unit Tests
	/// </summary>
	public class UnitTests_StateMachine
	{
		enum TestEnum
		{
			Invalid,
			Test1,
			Test2
		}

		private class TestStateMachine : StateMachine<TestEnum>
		{
			public int test1Count { get; protected set; }
			public int test2Count { get; protected set; }
			public int finishedCount { get; protected set; }

			protected override void OnStateFinish()
			{
				base.OnStateFinish();

				finishedCount++;
			}

			protected override void OnStateChange(TestEnum newState)
			{
				base.OnStateChange(newState);

				switch (newState) {
					case TestEnum.Test1:
						test1Count++;
						break;
					case TestEnum.Test2:
						test2Count++;
						break;
				}
			}
		}

		[Test]
		public void Test_StateMachine()
		{
			var test = new TestStateMachine();
			test.State = TestEnum.Test2;
			Assert.AreEqual(1, test.test2Count);
			test.IsLocked = true;

			test.State = TestEnum.Test1;
			Assert.AreEqual(0, test.test1Count);
			test.IsLocked = false;
			test.State = TestEnum.Test1;
			Assert.AreEqual(1, test.test1Count);

			test.SetStateDuration(1.0f);
			test.OnUpdate(new TimeSlice(1.5f));
			Assert.AreEqual(test.finishedCount, 1);
			test.ResetStateTimer();
			test.OnUpdate(new TimeSlice(.5f));
			Assert.AreEqual(.5f, test.StateProgress, .001f);
			test.OnUpdate(new TimeSlice(.6f));
			Assert.AreEqual(test.finishedCount, 2);

			test.State = TestEnum.Test2;
			Assert.AreEqual(TestEnum.Test1, test.prevState);
		}
    }
   }
