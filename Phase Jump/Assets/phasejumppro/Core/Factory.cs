namespace PJ
{
	/// <summary>
	/// Creates simple type
	/// </summary>
	public struct Factory<Type> : AnyFactory where Type : new()
	{
		public Type New()
		{
			return new Type();
		}
	}
}
