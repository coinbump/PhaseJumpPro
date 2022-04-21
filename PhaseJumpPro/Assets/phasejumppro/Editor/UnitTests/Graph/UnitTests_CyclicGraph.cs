using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_CyclicGraph
	{
		private class TestGraph : Graph.CyclicGraph<Graph.StandardEdgeModel>
        {
        }

		private class Node : Graph.CyclicNode<Graph.StandardEdgeModel>
		{
			public float time = 0;

			public override void OnUpdateNode(TimeSlice time)
			{
				this.time += time.delta;
			}
		}

        [Test]
        public void TestRemoveRoot_RootIsNull()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode);

            graph.RootNode = node;

            Assert.AreEqual(node, graph.RootNode);
            graph.Remove(node);
            Assert.AreEqual(null, graph.RootNode);
        }

        [Test]
        public void TestRemoveFromGraph_FromEdgesRemoved()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode);

            Assert.AreEqual(1, childNode.FromNodes.Count);

            graph.Remove(node);

            Assert.AreEqual(0, childNode.FromNodes.Count);
        }

        [Test]
        public void TestRemoveFromGraph_ToEdgesRemoved()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode);

            Assert.AreEqual(1, node.Edges.Count);

            graph.Remove(childNode);

            Assert.AreEqual(0, node.Edges.Count);
        }

        [Test]
        public void TestAddEdge_IsAdded()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode);

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
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

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
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

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
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);

            var delta = 4.0f;
            node.OnUpdate(new TimeSlice(delta));

            Assert.AreEqual(node.time, delta);
            Assert.AreEqual(childNode1.time, delta);
            Assert.AreEqual(childNode2.time, delta);
            Assert.AreEqual(deepNode.time, delta);
        }

        [Test]
        public void TestUpdateRootWithCircularReference_UpdatesAll()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);
            graph.AddEdge(deepNode, new Graph.StandardEdgeModel(), node);

            var delta = 4.0f;
            node.OnUpdate(new TimeSlice(delta));

            Assert.AreEqual(node.time, delta);
            Assert.AreEqual(childNode1.time, delta);
            Assert.AreEqual(childNode2.time, delta);
            Assert.AreEqual(deepNode.time, delta);
        }

        [Test]
        public void TestRemoveEdgeFromParent_RemovesBoth()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            Assert.AreEqual(node.Edges.Count, 1);
            Assert.AreEqual(childNode1.FromNodes.Count, 1);

            node.RemoveEdge(node.Edges[0]);

            Assert.AreEqual(node.Edges.Count, 0);
            Assert.AreEqual(childNode1.FromNodes.Count, 0);
        }

        [Test]
        public void TestRemoveEdgesTo()
        {
            var graph = new TestGraph();
            Node node = new Node();

            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

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
        public void TestRemoveEdgesFrom()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var deepNode = new Node();
            graph.AddEdge(deepNode, new Graph.StandardEdgeModel(), childNode1);

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
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);
            graph.AddEdge(deepNode, new Graph.StandardEdgeModel(), node);  // Circular connection

            var collectedGraph = node.CollectGraph();
            Assert.AreEqual(collectedGraph.Count, 4);
            Assert.IsTrue(collectedGraph.Contains(node));
            Assert.IsTrue(collectedGraph.Contains(childNode1));
            Assert.IsTrue(collectedGraph.Contains(childNode2));
            Assert.IsTrue(collectedGraph.Contains(deepNode));
        }

        [Test]
        public void TestCollectConnectedToNotDeep()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            childNode1.AddEdge(new Graph.StandardEdgeModel(), deepNode);

            var collectedGraph = node.CollectConnectedTo(false);
            Assert.AreEqual(collectedGraph.Count, 2);
            Assert.IsTrue(collectedGraph.Contains(childNode1));
            Assert.IsTrue(collectedGraph.Contains(childNode2));
        }

        [Test]
        public void TestCollectConnectedToDeep()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);

            var collectedGraph = node.CollectConnectedTo(true);
            Assert.AreEqual(collectedGraph.Count, 3);
            Assert.IsTrue(collectedGraph.Contains(childNode1));
            Assert.IsTrue(collectedGraph.Contains(childNode2));
            Assert.IsTrue(collectedGraph.Contains(deepNode));
        }

        [Test]
        public void TestCollectConnectedToCircular()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);
            graph.AddEdge(deepNode, new Graph.StandardEdgeModel(), node);  // Circular connection

            var collectedGraph = node.CollectConnectedTo(true);
            Assert.AreEqual(collectedGraph.Count, 4);
            Assert.IsTrue(collectedGraph.Contains(node));
            Assert.IsTrue(collectedGraph.Contains(childNode1));
            Assert.IsTrue(collectedGraph.Contains(childNode2));
            Assert.IsTrue(collectedGraph.Contains(deepNode));
        }

        [Test]
        public void TestCollectDepthFirstGraphTree()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);

            var collectedGraph = node.CollectDepthFirstGraph();
            Assert.AreEqual(collectedGraph.Count, 4);
            Assert.AreEqual(node, collectedGraph[0]);
            Assert.AreEqual(childNode1, collectedGraph[1]);
            Assert.AreEqual(deepNode, collectedGraph[2]);
            Assert.AreEqual(childNode2, collectedGraph[3]);
        }

        [Test]
        public void TestCollectDepthFirstGraphTree2()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode2, new Graph.StandardEdgeModel(), deepNode);

            var collectedGraph = node.CollectDepthFirstGraph();
            Assert.AreEqual(collectedGraph.Count, 4);
            Assert.AreEqual(node, collectedGraph[0]);
            Assert.AreEqual(childNode1, collectedGraph[1]);
            Assert.AreEqual(childNode2, collectedGraph[2]);
            Assert.AreEqual(deepNode, collectedGraph[3]);
        }

        [Test]
        public void TestCollectDepthFirstGraphCircular()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);
            graph.AddEdge(deepNode, new Graph.StandardEdgeModel(), node);  // Circular connection

            var collectedGraph = node.CollectDepthFirstGraph();
            Assert.AreEqual(collectedGraph.Count, 4);
            Assert.AreEqual(node, collectedGraph[0]);
            Assert.AreEqual(childNode1, collectedGraph[1]);
            Assert.AreEqual(deepNode, collectedGraph[2]);
            Assert.AreEqual(childNode2, collectedGraph[3]);
        }

        [Test]
        public void TestCollectBreadthFirstGraphTree()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);

            var collectedGraph = node.CollectBreadthFirstGraph();
            Assert.AreEqual(4, collectedGraph.Count);
            Assert.AreEqual(node, collectedGraph[0]);
            Assert.AreEqual(childNode1, collectedGraph[1]);
            Assert.AreEqual(childNode2, collectedGraph[2]);
            Assert.AreEqual(deepNode, collectedGraph[3]);
        }

        [Test]
        public void TestCollectBreadthFirstGraphTree2()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode2, new Graph.StandardEdgeModel(), deepNode);

            var collectedGraph = node.CollectBreadthFirstGraph();
            Assert.AreEqual(4, collectedGraph.Count);
            Assert.AreEqual(node, collectedGraph[0]);
            Assert.AreEqual(childNode1, collectedGraph[1]);
            Assert.AreEqual(childNode2, collectedGraph[2]);
            Assert.AreEqual(deepNode, collectedGraph[3]);
        }

        [Test]
        public void TestCollectBreadthFirstGraphCircular()
        {
            var graph = new TestGraph();
            Node node = new Node();
            var childNode1 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode1);

            var childNode2 = new Node();
            graph.AddEdge(node, new Graph.StandardEdgeModel(), childNode2);

            var deepNode = new Node();
            graph.AddEdge(childNode1, new Graph.StandardEdgeModel(), deepNode);
            graph.AddEdge(deepNode, new Graph.StandardEdgeModel(), node);  // Circular connection

            var collectedGraph = node.CollectBreadthFirstGraph();
            Assert.AreEqual(4, collectedGraph.Count);
            Assert.AreEqual(node, collectedGraph[0]);
            Assert.AreEqual(childNode1, collectedGraph[1]);
            Assert.AreEqual(childNode2, collectedGraph[2]);
            Assert.AreEqual(deepNode, collectedGraph[3]);
        }
    }
}
