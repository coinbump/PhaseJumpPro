using System;
using System.Collections.Generic;

namespace PJ {
	/// <summary>
	/// Node in a simulation graph
	/// Class, not struct for mutability
	/// </summary>
	public class SimNode: AnyGraphNode
	{
		private WeakReference<SimNode> parent;

		public string Identifier { get; set; }
		public string TypeIdentifier { get; set; }
		public string Title { get; set; }
		public string Description { get; set; }
		public Class Class { get; set; }

		// For tree-type graphs (optional)
		public AnyGraphNode Parent
		{
			get
			{
				parent.TryGetTarget(out SimNode result);
				return result;
			}
			set
			{
				if (value is SimNode simNode) {
					parent = new WeakReference<SimNode>(simNode);
				}
			}
		}

		public List<GraphConnection> Connections { get; set; }
	}
}
