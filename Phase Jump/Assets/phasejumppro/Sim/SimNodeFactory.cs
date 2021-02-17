
namespace PJ {
	/// <summary>
	/// Factory for sim nodes
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	public struct SimNodeFactory<Type> : AnyFactory where Type : SimNode, new()
	{
		Type New()
		{
			return new Type();
		}
	}
}
