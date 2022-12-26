using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_AcyclicGraphNode
	{
		private class Node : AcyclicGraphNode<StandardEdgeModel>
        {
			public float time = 0;
					
			public override void OnUpdate(TimeSlice time)
            {
                base.OnUpdate(time);
                this.time += time.delta;
            }
        }

		[Test]
		public void Test_AddEdge_IsAdded()
		{
			Node node = new Node();
			var childNode = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode);

			var connectedNodes = node.CollectConnectedTo(true);
			Assert.AreEqual(connectedNodes.Count, 1);
			Assert.IsTrue(connectedNodes.Contains(childNode));
			Assert.AreEqual(node.Edges.Count, 1);
			Assert.AreEqual(childNode.Edges.Count, 0);
			Assert.AreEqual(childNode.FromNodes.Count, 1);
		}

		[Test]
		public void Test_AddEdges_IsAdded()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

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
		public void Test_Clear_RemovesEdges()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

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
		public void Test_UpdateRoot_UpdatesAll()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);

			var delta = 4.0f;
			node.OnUpdate(new TimeSlice(delta));

			Assert.AreEqual(node.time, delta);
			Assert.AreEqual(childNode1.time, delta);
			Assert.AreEqual(childNode2.time, delta);
			Assert.AreEqual(deepNode.time, delta);
		}

		[Test]
		public void Test_UpdateRootWithCircularReference_DoesNotUpdateAll()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new StandardEdgeModel(), node);

			var delta = 4.0f;
			node.OnUpdate(new TimeSlice(delta));

			Assert.AreEqual(node.time, delta);
			Assert.AreEqual(childNode1.time, 0);
			Assert.AreEqual(childNode2.time, 0);
			Assert.AreEqual(deepNode.time, 0);
		}

		[Test]
		public void Test_RemoveEdgeFromParent_RemovesBoth()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			Assert.AreEqual(node.Edges.Count, 1);
			Assert.AreEqual(childNode1.FromNodes.Count, 1);

			node.RemoveEdge(node.Edges[0]);

			Assert.AreEqual(node.Edges.Count, 0);
			Assert.AreEqual(childNode1.FromNodes.Count, 0);
		}

		[Test]
		public void Test_RemoveEdgesTo()
		{
			Node node = new Node();

			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			Assert.AreEqual(node.Edges.Count, 2);
			Assert.AreEqual(childNode1.Edges.Count, 0);
			Assert.AreEqual(childNode1.FromNodes.Count, 1);
			Assert.AreEqual(childNode2.FromNodes.Count, 1);

			node.RemoveEdgesTo(childNode1);
			Assert.AreEqual(node.Edges.Count, 1);
			Assert.AreEqual(node.Edges[0].toNode.Value, childNode2);
			Assert.AreEqual(childNode1.FromNodes.Count, 0);
			Assert.AreEqual(childNode2.FromNodes.Count, 1);
		}

		[Test]
		public void Test_RemoveEdgesFrom()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var deepNode = new Node();
			deepNode.AddEdge(new StandardEdgeModel(), childNode1);

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
		public void Test_CollectGraph()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new StandardEdgeModel(), node);	// Circular connection

			var graph = node.CollectGraph();
			Assert.AreEqual(graph.Count, 4);
			Assert.IsTrue(graph.Contains(node));
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
			Assert.IsTrue(graph.Contains(deepNode));
		}

		[Test]
		public void Test_CollectConnectedToNotDeep()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);

			var graph = node.CollectConnectedTo(false);
			Assert.AreEqual(graph.Count, 2);
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
		}

		[Test]
		public void Test_CollectConnectedToDeep()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);

			var graph = node.CollectConnectedTo(true);
			Assert.AreEqual(graph.Count, 3);
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
			Assert.IsTrue(graph.Contains(deepNode));
		}

		[Test]
		public void Test_CollectConnectedToCircular()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new StandardEdgeModel(), node);	// Circular connection

			var graph = node.CollectConnectedTo(true);
			Assert.AreEqual(graph.Count, 4);
			Assert.IsTrue(graph.Contains(node));
			Assert.IsTrue(graph.Contains(childNode1));
			Assert.IsTrue(graph.Contains(childNode2));
			Assert.IsTrue(graph.Contains(deepNode));
		}

		[Test]
		public void Test_CollectDepthFirstGraphTree()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);

			var graph = node.CollectDepthFirstGraph();
			Assert.AreEqual(graph.Count, 4);
			Assert.AreEqual(node, graph[0]);
			Assert.AreEqual(childNode1, graph[1]);
			Assert.AreEqual(deepNode, graph[2]);
			Assert.AreEqual(childNode2, graph[3]);
		}

		[Test]
		public void Test_CollectDepthFirstGraphTree2()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode2.AddEdge(new StandardEdgeModel(), deepNode);

			var graph = node.CollectDepthFirstGraph();
			Assert.AreEqual(graph.Count, 4);
			Assert.AreEqual(node, graph[0]);
			Assert.AreEqual(childNode1, graph[1]);
			Assert.AreEqual(childNode2, graph[2]);
			Assert.AreEqual(deepNode, graph[3]);
		}

		[Test]
		public void Test_CollectDepthFirstGraphCircular()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new StandardEdgeModel(), node);  // Circular connection

			var graph = node.CollectDepthFirstGraph();
			Assert.AreEqual(graph.Count, 4);
			Assert.AreEqual(node, graph[0]);
			Assert.AreEqual(childNode1, graph[1]);
			Assert.AreEqual(deepNode, graph[2]);
			Assert.AreEqual(childNode2, graph[3]);
		}

		[Test]
		public void Test_CollectBreadthFirstGraphTree()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);

			var graph = node.CollectBreadthFirstGraph();
			Assert.AreEqual(4, graph.Count);
			Assert.AreEqual(node, graph[0]);
			Assert.AreEqual(childNode1, graph[1]);
			Assert.AreEqual(childNode2, graph[2]);
			Assert.AreEqual(deepNode, graph[3]);
		}

		[Test]
		public void Test_CollectBreadthFirstGraphTree2()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode2.AddEdge(new StandardEdgeModel(), deepNode);

			var graph = node.CollectBreadthFirstGraph();
			Assert.AreEqual(4, graph.Count);
			Assert.AreEqual(node, graph[0]);
			Assert.AreEqual(childNode1, graph[1]);
			Assert.AreEqual(childNode2, graph[2]);
			Assert.AreEqual(deepNode, graph[3]);
		}

		[Test]
		public void Test_CollectBreadthFirstGraphCircular()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new StandardEdgeModel(), node);  // Circular connection

			var graph = node.CollectBreadthFirstGraph();
			Assert.AreEqual(4, graph.Count);
			Assert.AreEqual(node, graph[0]);
			Assert.AreEqual(childNode1, graph[1]);
			Assert.AreEqual(childNode2, graph[2]);
			Assert.AreEqual(deepNode, graph[3]);
		}

		[Test]
		public void Test_RootNode()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);

			Assert.AreEqual(node, node.RootNode);
			Assert.AreEqual(node, childNode1.RootNode);
			Assert.AreEqual(node, childNode2.RootNode);
			Assert.AreEqual(node, deepNode.RootNode);
		}

		[Test]
		public void Test_RootNodeCircular()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			var childNode2 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode2);

			var deepNode = new Node();
			childNode1.AddEdge(new StandardEdgeModel(), deepNode);
			deepNode.AddEdge(new StandardEdgeModel(), node);

			Assert.AreEqual(null, node.RootNode);
			Assert.AreEqual(null, childNode2.RootNode);
			Assert.AreEqual(null, childNode1.RootNode);
			Assert.AreEqual(null, deepNode.RootNode);
		}

		[Test]
		public void Test_IsRootNode()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			Assert.IsTrue(node.IsRootNode);
			Assert.IsFalse(childNode1.IsRootNode);
		}

		[Test]
		public void Test_Parent()
		{
			Node node = new Node();
			var childNode1 = new Node();
			node.AddEdge(new StandardEdgeModel(), childNode1);

			Assert.AreEqual(null, node.Parent);
			Assert.AreEqual(node, childNode1.Parent);
		}
	}
}
