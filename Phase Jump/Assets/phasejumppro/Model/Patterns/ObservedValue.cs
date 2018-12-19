using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple design pattern with Unit Test
 * CODE REVIEW: 4.9.18
 */
namespace PJ
{
	/// <summary>
	/// Broadcasts when value changes.
	/// </summary>
	public class ObservedValue<T>
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

		// Conversion operator
		public static explicit operator T(ObservedValue<T> value)
		{
			return value.Value;
		}

		public ObservedValue() {
			
		}

		public ObservedValue(Action<T> action) {	
			this.action = action;
		}

		protected virtual void EvtValueChanged()
		{
			action?.Invoke(Value);

			broadcaster.Broadcast(new Event(PJ.EventNames.ValueChanged));
		}
	}
	
}
