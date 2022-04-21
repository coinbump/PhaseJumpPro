using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple design pattern with Unit Test
 * CODE REVIEW: 4/13/22
 */
namespace PJ
{
	/// <summary>
    /// Type erasure
    /// </summary>
	public abstract class AnyPublishedValue
    {
    }

	/// <summary>
	/// Broadcasts when value changes.
	/// </summary>
	// FUTURE: support binding value A to B if needed
	public class PublishedValue<T> : AnyPublishedValue
	{
		/// <summary>
        /// Sent when a value changes
        /// </summary>
		public class EventValueChange : Event
		{
			public PublishedValue<T> value;

			public EventValueChange(PublishedValue<T> value)
            {
				this.value = value;
            }
		}

		public void OnValueChange(Event theEvent, Action<T> action)
        {
			var eventValueChange = theEvent as EventValueChange;
			if (null != eventValueChange && eventValueChange.value == this)
            {
				action(value);
            }
        }

		protected T value;
		public Broadcaster broadcaster = new Broadcaster();

		public T Value
		{
			get => value;
			set
			{
				if (!value.Equals(this.value))
				{
					this.value = value;
					OnValueChange();
				}
			}
		}

		public void AddListener(SomeListener listener)
        {
			broadcaster.AddListener(listener);
        }

		/// <summary>
		/// Conversion operator
		/// </summary>
		public static explicit operator T(PublishedValue<T> value)
		{
			return value.Value;
		}

		public PublishedValue()
		{
		}

		public PublishedValue(T value)
		{
			this.value = value;
		}

		protected virtual void OnValueChange()
		{
			broadcaster.Broadcast(new EventValueChange(this));
		}
	}
}
