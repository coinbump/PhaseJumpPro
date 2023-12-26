using System;
using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Simple design pattern with Unit Test
 * CODE REVIEW: 11/5/22
 * PORTED TO: C++
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

    public abstract class SomePublished<T> : AnyPublished
    {
        public abstract T Value { get; set; }

        public abstract void AddListener(SomeListener listener);
    }

    /// <summary>
    /// Broadcasts when value changes.
    /// </summary>
    public class PublishedEntity<T> : SomePublished<T>
    {
        protected T value;
        public Broadcaster broadcaster = new Broadcaster();

        protected Func<T, T, bool> evaluateIsEqual;

        public override T Value
        {
            get => value;
            set
            {
                var oldValue = this.value;
                this.value = value;

                if (null != evaluateIsEqual)
                {
                    if (evaluateIsEqual(oldValue, this.value)) { return; }
                }

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

        public override void AddListener(SomeListener listener)
        {
            broadcaster.AddListener(listener);
        }

        /// <summary>
        /// Set value without publishing change
        /// </summary>
        public void SetValueSilent(T value)
        {
            this.value = value;
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
        /// <summary>
        /// Conversion operator
        /// </summary>
        public static explicit operator T(PublishedValue<T> value)
        {
            return value.Value;
        }

        public PublishedValue()
        {
            evaluateIsEqual = EvaluateIsEqual;
        }

        public PublishedValue(T value)
        {
            this.value = value;
            evaluateIsEqual = EvaluateIsEqual;
        }

        protected bool EvaluateIsEqual(T oldValue, T newValue)
        {
            var isNullToValueChange = (oldValue == null && newValue != null) || (oldValue != null && newValue == null);
            var isChange = isNullToValueChange || !newValue.Equals(oldValue);
            return !isChange;
        }

        public bool Equals(T value)
        {
            var myValue = Value;
            if (null == myValue && null == value) { return true; }
            if (null == myValue || null == value) { return false; }

            return myValue.Equals(value);
        }
    }

    public class PublishedTransformValue<T> : PublishedValue<T> where T : IEquatable<T>
    {
        public SomeValueTransform<T> transform = new IdentityTransform<T>();

        public override T Value
        {
            get => value;
            set
            {
                var oldValue = this.value;
                this.value = transform.Transform(value);
                if (evaluateIsEqual(oldValue, this.value)) { return; }

                OnValueChange();
            }
        }

        public PublishedTransformValue()
        {
        }

        public PublishedTransformValue(T value) : base(value)
        {
        }
    }
}
