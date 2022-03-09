using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_GraphNode
	{
		private class Node : Graph.Node<Graph.StandardEdgeModel>
        {
			public float time = 0;

			public override void EvtUpdate(TimeSlice time)
            {
				this.time += time.delta;
            }
        }

		[Test]
		public void TestAddEdge_IsAdded()
		{
			Node node = new Node();
			var childNode = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode);

			var connectedNodes = node.CollectConnectedTo(true);
			Assert.AreEqual(connectedNodes.Count, 1);
			Assert.IsTrue(connectedNodes.Contains(childNode));
			Assert.AreEqual(node.Edges.Count, 1);
			Assert.AreEqual(childNode.Edges.Count, 0);
			Assert.AreEqual(childNode.FromNodes.Count, 1);
		}

		[Test]
		public void TestAddEdges_IsAdded()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			var connectedNodes = node.CollectConnectedTo(true);
			Assert.AreEqual(connectedNodes.Count, 2);
			Assert.IsTrue(connectedNodes.Contains(childNode1));
			Assert.IsTrue(connectedNodes.Contains(childNode2));
			Assert.AreEqual(node.Edges.Count, 2);
			Assert.AreEqual(childNode1.Edges.Count, 0);
			Assert.AreEqual(childNode1.FromNodes.Count, 1);
			Assert.AreEqual(childNode2.Edges.Count, 0);
			Assert.AreEqual(childNode2.FromNodes.Count, 1);
		}

		[Test]
		public void TestClear_RemovesEdges()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			Assert.AreEqual(childNode1.FromNodes.Count, 1);
			Assert.AreEqual(childNode2.FromNodes.Count, 1);

			node.Clear();
            var connectedNodes = node.CollectConnectedTo(true);
            Assert.AreEqual(connectedNodes.Count, 0);
			Assert.AreEqual(node.Edges.Count, 0);
            Assert.AreEqual(childNode1.FromNodes.Count, 0);
            Assert.AreEqual(childNode2.FromNodes.Count, 0);
        }

        [Test]
		public void TestUpdateRoot_UpdatesAll()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new Graph.StandardEdgeModel(), deepNode);

			var delta = 4.0f;
			node.EvtUpdateRoot(new TimeSlice(delta));

			Assert.AreEqual(node.time, delta);
			Assert.AreEqual(childNode1.time, delta);
			Assert.AreEqual(childNode2.time, delta);
			Assert.AreEqual(deepNode.time, delta);
		}

		[Test]
		public void TestUpdateRootWithCircularReference_UpdatesAll()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new Graph.StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new Graph.StandardEdgeModel(), node);

			var delta = 4.0f;
			node.EvtUpdateRoot(new TimeSlice(delta));

			Assert.AreEqual(node.time, delta);
			Assert.AreEqual(childNode1.time, delta);
			Assert.AreEqual(childNode2.time, delta);
			Assert.AreEqual(deepNode.time, delta);
		}

		[Test]
		public void TestRemoveEdgeFromParent_RemovesBoth()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			Assert.AreEqual(node.Edges.Count, 1);
			Assert.AreEqual(childNode1.FromNodes.Count, 1);

			node.RemoveEdge(node.Edges[0]);

			Assert.AreEqual(node.Edges.Count, 0);
			Assert.AreEqual(childNode1.FromNodes.Count, 0);
		}

		[Test]
		public void TestRemoveEdgesTo()
		{
			Node node = new Node();

			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			Assert.AreEqual(node.Edges.Count, 2);
			Assert.AreEqual(childNode1.Edges.Count, 0);
			Assert.AreEqual(childNode1.FromNodes.Count, 1);
			Assert.AreEqual(childNode2.FromNodes.Count, 1);

			node.RemoveEdgesTo(childNode1);
			Assert.AreEqual(node.Edges.Count, 1);
			Assert.AreEqual(node.Edges[0].toNode, childNode2);
			Assert.AreEqual(childNode1.FromNodes.Count, 0);
			Assert.AreEqual(childNode2.FromNodes.Count, 1);
		}

		[Test]
		public void TestRemoveEdgesFrom()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var deepNode = new Node();
			deepNode.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			Assert.AreEqual(node.Edges.Count, 1);
			Assert.AreEqual(childNode1.FromNodes.Count, 2);
			Assert.AreEqual(deepNode.Edges.Count, 1);
			Assert.AreEqual(deepNode.FromNodes.Count, 0);

			childNode1.RemoveEdgesFrom(node);

			Assert.AreEqual(node.Edges.Count, 0);
			Assert.AreEqual(childNode1.FromNodes.Count, 1);
			Assert.AreEqual(deepNode.Edges.Count, 1);
			Assert.AreEqual(deepNode.FromNodes.Count, 0);

			childNode1.RemoveEdgesFrom(deepNode);

			Assert.AreEqual(node.Edges.Count, 0);
			Assert.AreEqual(childNode1.Edges.Count, 0);
			Assert.AreEqual(childNode1.FromNodes.Count, 0);
			Assert.AreEqual(deepNode.Edges.Count, 0);
		}

		[Test]
		public void TestCollectGraph()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new Graph.StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new Graph.StandardEdgeModel(), node);	// Circular connection

			var graph = node.CollectGraph();
			Assert.AreEqual(graph.Count, 4);
			Assert.IsTrue(graph.Contains(node));
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
			Assert.IsTrue(graph.Contains(deepNode));
		}

		[Test]
		public void TestCollectConnectedToNotDeep()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new Graph.StandardEdgeModel(), deepNode);

			var graph = node.CollectConnectedTo(false);
			Assert.AreEqual(graph.Count, 2);
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
		}

		[Test]
		public void TestCollectConnectedToDeep()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new Graph.StandardEdgeModel(), deepNode);

			var graph = node.CollectConnectedTo(true);
			Assert.AreEqual(graph.Count, 3);
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
			Assert.IsTrue(graph.Contains(deepNode));
		}

		[Test]
		public void TestCollectConnectedToCircular()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new Graph.StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new Graph.StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new Graph.StandardEdgeModel(), node);	// Circular connection

			var graph = node.CollectConnectedTo(true);
			Assert.AreEqual(graph.Count, 4);
			Assert.IsTrue(graph.Contains(node));
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
			Assert.IsTrue(graph.Contains(deepNode));
		}
	}
}
