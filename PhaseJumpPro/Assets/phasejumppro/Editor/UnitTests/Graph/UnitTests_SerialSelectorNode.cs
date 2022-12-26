using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_SerialSelectorNode
	{
		[Test]
		public void UnitTests()
		{
			Graph.SerialSelectorNode<StandardEdgeModel> node = new();

			var childNode1 = new AcyclicGraphNode<StandardEdgeModel>();
			childNode1.id = "1";
			node.AddEdge(new(), childNode1);

			var childNode2 = new AcyclicGraphNode<StandardEdgeModel>();
			childNode2.id = "2";
			node.AddEdge(new(), childNode2);

			var nextNode = node.SelectNext();
			Assert.AreEqual(nextNode.id, "1");

			nextNode = node.SelectNext();
			Assert.AreEqual(nextNode.id, "2");

			nextNode = node.SelectNext();
			Assert.AreEqual(nextNode.id, "1");
		}
	}
}
