using System.Collections.Generic;

namespace PJ
{
	/// <summary>
	/// A node in a graph
	/// </summary>
	public interface AnyGraphNode
	{
		string Identifier { get; set; }

		// For tree-type graphs (optional)
		AnyGraphNode Parent { get; set; }

		// List, not set because order might matter.
		List<GraphConnection> Connections { get; set; }
	}
}
