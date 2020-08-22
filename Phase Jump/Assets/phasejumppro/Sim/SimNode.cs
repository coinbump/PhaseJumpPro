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

		public string typeIdentifier;

		public string Identifier { get; set; }

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
