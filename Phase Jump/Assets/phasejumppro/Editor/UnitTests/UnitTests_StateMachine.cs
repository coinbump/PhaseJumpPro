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

		private class TestStateMachine : GenericStateMachine<TestEnum>
		{
			public int test1Count { get; protected set; }
			public int test2Count { get; protected set; }
			public int finishedCount { get; protected set; }

			protected override void EvtStateFinished()
			{
				base.EvtStateFinished();

				finishedCount++;
			}

			protected override void EvtStateChanged(TestEnum newState)
			{
				base.EvtStateChanged(newState);

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
		public void UnitTests()
		{
			var test = new TestStateMachine();
			test.State = TestEnum.Test2;
			Assert.AreEqual(1, test.test2Count);
			test.Lock(true);

			test.State = TestEnum.Test1;
			Assert.AreEqual(0, test.test1Count);
			test.Lock(false);
			test.State = TestEnum.Test1;
			Assert.AreEqual(1, test.test1Count);

			test.SetStateDuration(1.0f);
			test.EvtUpdate(new TimeSlice(1.5f));
			Assert.AreEqual(test.finishedCount, 1);
			test.ResetStateTimer();
			test.EvtUpdate(new TimeSlice(.5f));
			Assert.AreEqual(.5f, test.Progress, .001f);
			test.EvtUpdate(new TimeSlice(.6f));
			Assert.AreEqual(test.finishedCount, 2);

			test.State = TestEnum.Test2;
			Assert.AreEqual(TestEnum.Test1, test.prevState);
		}

    }
    }
