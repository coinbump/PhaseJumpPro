using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_Map1DToMap2DNode
	{
		private class Node : PJ.Graph.GoStandardNode
		{
		}

		[Test]
		public void TestMap_XAxis()
		{
			Graph.SinMap1DNode map1DNode = new Graph.SinMap1DNode(false, 0);
			Graph.Map1DToMap2DNode transformNode = new Graph.Map1DToMap2DNode (Axis2D.X);
			map1DNode.AddOutput("value", transformNode, "value");

			SomeMap<Vector2, float> map = transformNode.ValueForOutput<SomeMap<Vector2, float>>("value", null, null) as SomeMap<Vector2, float>;
			Assert.NotNull(map);

			Assert.AreEqual(Mathf.Sin(0), map.ValueFor(new Vector2(0, 0.5f)));
			Assert.AreEqual(Mathf.Sin(0.3f * 2.0f * Mathf.PI), map.ValueFor(new Vector2(0.3f, 0.5f)), .001f);
		}

		[Test]
		public void TestMap_YAxis()
		{
			Graph.SinMap1DNode map1DNode = new Graph.SinMap1DNode(false, 0);
			Graph.Map1DToMap2DNode transformNode = new Graph.Map1DToMap2DNode(Axis2D.Y);
			map1DNode.AddOutput("value", transformNode, "value");

			SomeMap<Vector2, float> map = transformNode.ValueForOutput<SomeMap<Vector2, float>>("value", null, null) as SomeMap<Vector2, float>;
			Assert.NotNull(map);

			Assert.AreEqual(Mathf.Sin(0), map.ValueFor(new Vector2(0.5f, 0)));
			Assert.AreEqual(Mathf.Sin(0.3f * 2.0f * Mathf.PI), map.ValueFor(new Vector2(0.5f, 0.3f)), .001f);
		}
	}
}
