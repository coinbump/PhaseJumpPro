
namespace PJ
{
	public class GenericFactory<T> where T : new()
	{
		public virtual T FactoryNew() { return new T(); }
	}
}
