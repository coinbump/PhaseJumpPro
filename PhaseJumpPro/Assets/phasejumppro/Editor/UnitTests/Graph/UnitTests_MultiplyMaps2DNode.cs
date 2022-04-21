using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_MultiplyMaps2DNode
	{
		private class Node : PJ.Graph.GoStandardAcyclicNode
		{
		}

		[Test]
		public void TestMap()
		{
			Graph.SinMap1DNode map1DNode = new Graph.SinMap1DNode(false, 0);
			Graph.Map1DToMap2DNode transformNode = new Graph.Map1DToMap2DNode(Axis2D.X);
			map1DNode.AddOutput("value", transformNode, "value");

			var multiplyNode = new Graph.MultiplyMaps2DNode();
			transformNode.AddOutput("value", multiplyNode, "map-1");
			transformNode.AddOutput("value", multiplyNode, "map-2");

			SomeMap<Vector2, float> map = multiplyNode.ValueForOutput<SomeMap<Vector2, float>>("value", null, null) as SomeMap<Vector2, float>;
			Assert.NotNull(map);

			Assert.AreEqual(Mathf.Sin(0) * Mathf.Sin(0), map.ValueFor(new Vector2(0, 0.5f)));
			Assert.AreEqual(Mathf.Sin(0.3f * 2.0f * Mathf.PI) * Mathf.Sin(0.3f * 2.0f * Mathf.PI), map.ValueFor(new Vector2(0.3f, 0.5f)), .001f);
		}
	}
}
