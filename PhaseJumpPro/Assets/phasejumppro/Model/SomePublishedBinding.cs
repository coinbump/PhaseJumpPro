using System;

/*
 * RATING: 5 stars
 * Simple design pattern with Unit Test
 * CODE REVIEW: 11/5/22
 */
namespace PJ
{
    /// <summary>
    /// A binding transforms one published value to another
    /// Example: Int: 10 -> String: "ten"
    /// </summary>
    public abstract class SomePublishedBinding<FromType, ToType> : SomeListener
    {
        protected WeakReference<SomePublished<FromType>> fromValue;
        protected WeakReference<SomePublished<ToType>> toValue;

        public SomePublishedBinding(SomePublished<FromType> fromValue, SomePublished<ToType> toValue)
        {
            if (null == fromValue || null == toValue) { return; }

            toValue.Value = Transform(fromValue.Value);

            // Avoid strong reference captures (binding doesn't claim ownership of objects it binds to)
            this.fromValue = new WeakReference<SomePublished<FromType>>(fromValue);
            fromValue.AddListener(this);

            this.toValue = new WeakReference<SomePublished<ToType>>(toValue);
        }

        public void OnListen(PJ.Event theEvent)
        {
            if (fromValue.TryGetTarget(out SomePublished<FromType> fromTarget) &&
                toValue.TryGetTarget(out SomePublished<ToType> toTarget))
            {
                var valueChangeEvent = theEvent as EventPublishedChange<FromType>;
                if (null != valueChangeEvent && valueChangeEvent.value == fromTarget)
                {
                    var newValue = Transform(fromTarget.Value);
                    toTarget.Value = newValue;
                }
            }
        }

        public abstract ToType Transform(FromType fromValue);
    }

    public class StringBinding : SomePublishedBinding<string, string>
    {
        public override string Transform(string fromValue) => fromValue;

        public StringBinding(SomePublished<string> fromValue, SomePublished<string> toValue) : base(fromValue, toValue)
        {
        }
    }

    public class IntToStringBinding : SomePublishedBinding<int, string>
    {
        public override string Transform(int fromValue) => fromValue.ToString();

        public IntToStringBinding(SomePublished<int> fromValue, SomePublished<string> toValue) : base(fromValue, toValue)
        {
        }
    }
}
