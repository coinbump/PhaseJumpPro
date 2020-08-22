
namespace PJ {
	/// <summary>
	/// Factory for sim nodes
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	public struct SimNodeFactory<Type> : AnyFactory<Type> where Type : SimNode, new()
	{
		Type AnyFactory<Type>.New()
		{
			return new Type();
		}
	}
}
