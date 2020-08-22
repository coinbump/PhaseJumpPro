namespace PJ
{
	/// <summary>
	/// Creates simple type
	/// </summary>
	struct Factory<Type> : AnyFactory<Type> where Type : new()
	{
		Type AnyFactory<Type>.New()
		{
			return new Type();
		}
	}
}
