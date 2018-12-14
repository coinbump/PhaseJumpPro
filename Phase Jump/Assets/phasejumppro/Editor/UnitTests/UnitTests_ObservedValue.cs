using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	class UnitTests_ObservedValue
	{
		private class TestValue : ObservedValue<int>
		{
			public TestValue()
			{
				Action<int> _action = _value => { actionCount++; };
				this.action = _action;
			}

			protected override void EvtValueChanged()
			{
				base.EvtValueChanged();

				changedCount++;
			}

			public int changedCount;
			public int actionCount;
		}

		[Test]
		public void UnitTests()
		{
			var test = new TestValue();
			Assert.AreEqual(0, test.Value);
			test.Value = 10;
			Assert.AreEqual(1, test.changedCount);
			Assert.AreEqual(1, test.actionCount);
		}
	}
}