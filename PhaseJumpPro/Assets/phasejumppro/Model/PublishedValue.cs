using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static UnityEngine.Rendering.DebugUI;

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
    public class PublishedEntity<T> : AnyPublishedValue
    {
        protected T value;
        public Broadcaster broadcaster = new Broadcaster();

        public virtual T Value
        {
            get => value;
            set
            {
                OnValueChange();
            }
        }

        /// <summary>
        /// Sent when a value changes
        /// </summary>
        public class EventValueChange : Event
        {
            public PublishedEntity<T> value;

            public EventValueChange(PublishedEntity<T> value)
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

        protected virtual void OnValueChange()
        {
            broadcaster.Broadcast(new EventValueChange(this));
        }
    }

    /// <summary>
    /// Broadcasts when value changes, with Equatable requirement.
    /// </summary>
    public class PublishedValue<T> : PublishedEntity<T> where T : IEquatable<T>
	{
		public override T Value
		{
			get => value;
			set
			{
				var isNullToValueChange = (this.value == null && value != null) || (this.value != null && value == null);

				if (isNullToValueChange || !value.Equals(this.value))
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

		public bool Equals(T value)
        {
			var myValue = Value;
			if (null == myValue && null == value) { return true; }
            if (null == myValue || null == value) { return false; }

            return myValue.Equals(value);
        }
    }
}
