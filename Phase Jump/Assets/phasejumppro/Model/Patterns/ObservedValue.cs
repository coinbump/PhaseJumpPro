using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

/*
 * RATING: 5 stars. Simple design pattern with Unit Test
 * CODE REVIEW: 4.9.18
 */
namespace PJ
{
	/// <summary>
	/// Broadcasts when value changes.
	/// </summary>
	class ObservedValue<T>
	{
		// FUTURE: support binding value A to B
		private T _value;
		public Broadcaster broadcaster = new Broadcaster();
		protected Action<T> action;

		public T Value
		{
			get
			{
				return _value;
			}
			set
			{
				if (!value.Equals(_value))
				{
					_value = value;
					EvtValueChanged();
				}
			}
		}

		public ObservedValue() {
			
		}

		public ObservedValue(Action<T> action) {	
			this.action = action;
		}

		protected virtual void EvtValueChanged()
		{
			if (action != null) {
				action(Value);
			}

			broadcaster.Broadcast(new Event(PJ.EventNames.ValueChanged));
		}
	}

	class UnitTests_ObservedValue {
		class TestValue : ObservedValue<int> 
		{
			public TestValue() {
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
		public void UnitTests() {
			var test = new TestValue();
			Assert.AreEqual(0, test.Value);
			test.Value = 10;
			Assert.AreEqual(1, test.changedCount);
			Assert.AreEqual(1, test.actionCount);
		}
	}
	
}
