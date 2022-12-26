using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_RandomSelectorNode
	{
		[Test]
		public void UnitTests()
		{
			Graph.RandomSelectorNode<StandardEdgeModel> node = new();
			var childNode = new Graph.RandomSelectorNode<StandardEdgeModel>();
			childNode.id = "test";
			node.AddEdge(new(), childNode);

			Assert.AreEqual(node.SelectNext().id, childNode.id);
			Assert.AreEqual(node.SelectNext().id, childNode.id);

			node.Clear();
            var nextNull = node.SelectNext();
            Assert.AreEqual(nextNull, null);
        }
    }
}
