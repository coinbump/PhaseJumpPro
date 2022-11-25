using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple design pattern with Unit Test
 * CODE REVIEW: 11/5/22
 */
namespace PJ
{
    /// <summary>
    /// Sent when a value changes
    /// </summary>
    public class EventPublishedChange<T> : Event
    {
        public PublishedEntity<T> value;

        public EventPublishedChange(PublishedEntity<T> value)
        {
            this.value = value;
        }
    }

    /// <summary>
    /// Used for type erasure to store Published value in a collection
    /// </summary>
    public abstract class AnyPublished
    {
    }

    public interface SomePublished<T>
    {
        T Value { get; set; }

        void AddListener(SomeListener listener);
    }

    /// <summary>
    /// Broadcasts when value changes.
    /// </summary>
    public class PublishedEntity<T> : AnyPublished, SomePublished<T>
    {
        protected T value;
        public Broadcaster broadcaster = new Broadcaster();
        public SomeTransform<T> transform = new IdentityTransform<T>();

        public virtual T Value
        {
            get => value;
            set
            {
                this.value = transform.Transform(value);

                // Broadcast without checking if value has changed
                // If you need an equality check for value changes, use PublishedValue
                OnValueChange();
            }
        }

        public PublishedEntity()
        {
        }

        public PublishedEntity(T value)
        {
            this.value = value;
        }

        public void AddListener(SomeListener listener)
        {
            broadcaster.AddListener(listener);
        }

        /// <summary>
        /// Convenience function for responding to a value change event
        /// Checks if the value change belongs to this Published object and performs the action if it is
        /// </summary>
        /// <param name="theEvent">Event sent by broadcaster to listener</param>
        /// <param name="action">Action to perform if event belongs to this Published object</param>
        public void OnValueChange(Event theEvent, Action<T> action)
        {
            var eventValueChange = theEvent as EventPublishedChange<T>;
            if (null != eventValueChange && eventValueChange.value == this)
            {
                action(value);
            }
        }

        protected virtual void OnValueChange()
        {
            broadcaster.Broadcast(new EventPublishedChange<T>(this));
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
                var newValue = transform.Transform(value);
				var isNullToValueChange = (this.value == null && newValue != null) || (this.value != null && newValue == null);

				if (isNullToValueChange || !newValue.Equals(this.value))
				{
                    this.value = newValue;
                    OnValueChange();
				}
			}
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
