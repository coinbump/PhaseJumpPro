namespace PJ
{
	public abstract class SomeFactory: AnyFactory
    {
		public abstract object New();
    }

	/// <summary>
	/// Creates simple type
	/// </summary>
	public class Factory<Type> : SomeFactory where Type : new()
	{
		public Type NewType()
		{
			return new Type();
		}

        public override object New()
        {
			return NewType();
        }
	}
}
