using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_GoStandardNode
	{
		private class Node : PJ.Graph.GoStandardAcyclicNode
		{
		}

		[Test]
		public void TestAddOutput_IsAdded()
		{
			Node node = new Node();
			Node childNode = new Node();

			node.AddOutput("output", childNode, "input");

			Assert.AreEqual(1, node.Edges.Count);
			Assert.AreEqual(0, childNode.Edges.Count);
			Assert.AreEqual(1, childNode.FromNodes.Count);
		}

		[Test]
		public void TestFindOutputEdge()
		{
			Node node = new Node();
			Node childNode1 = new Node();
			Node childNode2 = new Node();

			node.AddOutput("output-1", childNode1, "input");
			node.AddOutput("output-2", childNode2, "input");

			Assert.AreEqual(node.Edges[0], node.OutputEdgeFor("output-1"));
			Assert.AreEqual(node.Edges[1], node.OutputEdgeFor("output-2"));
		}

		[Test]
		public void TestFindOutputNode()
		{
			Node node = new Node();
			Node childNode1 = new Node();
			Node childNode2 = new Node();

			node.AddOutput("output-1", childNode1, "input");
			node.AddOutput("output-2", childNode2, "input");

			Assert.AreEqual(childNode1, node.OutputNodeFor("output-1"));
			Assert.AreEqual(childNode2, node.OutputNodeFor("output-2"));
		}

		[Test]
		public void TestFindInputNode_CorrectInput()
		{
			Node node = new Node();
			Node childNode1 = new Node();
			Node childNode2 = new Node();

			node.AddOutput("output-1", childNode1, "input-1");
			node.AddOutput("output-2", childNode2, "input-2");

			Assert.AreEqual(node, childNode1.InputNodeFor("input-1"));
			Assert.AreEqual(node, childNode2.InputNodeFor("input-2"));
		}

		[Test]
		public void TestFindInputNode_WrongInput()
		{
			Node node = new Node();
			Node childNode1 = new Node();
			Node childNode2 = new Node();

			node.AddOutput("output-1", childNode1, "input-1");
			node.AddOutput("output-2", childNode2, "input-2");

			Assert.AreEqual(null, childNode1.InputNodeFor("input-2"));
			Assert.AreEqual(null, childNode2.InputNodeFor("input-1"));
		}
	}
}
