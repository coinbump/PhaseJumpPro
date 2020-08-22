namespace PJ
{
	/// <summary>
	/// Abstracts factory implementation
	/// </summary>
	interface AnyFactory<Type>
	{
		Type New();
	}
}
