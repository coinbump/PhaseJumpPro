using System;

namespace PJ
{
	/// <summary>
	/// A binding transforms one published value into another
	/// Example: Int: 10 -> String: "ten"
	/// </summary>
	public abstract class Binding<FromType, ToType> : SomeListener
	{
		protected WeakReference<PublishedValue<FromType>> fromValue;
		protected WeakReference<PublishedValue<ToType>> toValue;

		public Binding(PublishedValue<FromType> fromValue, PublishedValue<ToType> toValue)
		{
			if (null == fromValue || null == toValue) { return; }

			toValue.Value = Transform(fromValue.Value);

			// Avoid strong reference captures (binding doesn't claim ownership of objects it binds to)
			this.fromValue = new WeakReference<PublishedValue<FromType>>(fromValue);
			fromValue.AddListener(this);

			this.toValue = new WeakReference<PublishedValue<ToType>>(toValue);
		}

		public void OnListen(PJ.Event theEvent)
		{
			if (fromValue.TryGetTarget(out PublishedValue<FromType> fromTarget) &&
				toValue.TryGetTarget(out PublishedValue<ToType> toTarget))
			{
				var valueChangeEvent = theEvent as PublishedValue<FromType>.EventValueChange;
				if (null != valueChangeEvent && valueChangeEvent.value == fromTarget)
				{
					var newValue = Transform(fromTarget.Value);
					toTarget.Value = newValue;
				}
			}
		}

		public abstract ToType Transform(FromType fromValue);
	}

	public class StringBinding : Binding<string, string>
	{
		public override string Transform(string fromValue) => fromValue;

		public StringBinding(PublishedValue<string> fromValue, PublishedValue<string> toValue) : base(fromValue, toValue)
		{
		}
	}

	public class IntToStringBinding : Binding<int, string>
	{
		public override string Transform(int fromValue) => fromValue.ToString();

		public IntToStringBinding(PublishedValue<int> fromValue, PublishedValue<string> toValue) : base(fromValue, toValue)
		{
		}
	}
}
