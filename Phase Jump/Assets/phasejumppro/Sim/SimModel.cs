namespace PJ
{
	/// <summary>
	/// A simulation model holds a graph of nodes.
	/// </summary>
	public interface SimModel
	{
		SimNode RootNode { get; }

		void AddNode(SimNode node);
		void AddConnection(GraphConnection connection, SimNode fromNode);
	}
}
