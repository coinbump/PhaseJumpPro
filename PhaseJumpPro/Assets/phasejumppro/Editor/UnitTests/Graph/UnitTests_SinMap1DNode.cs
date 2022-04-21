using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_SinMap1DNode
	{
		private class Node : PJ.Graph.GoStandardAcyclicNode
		{
		}

		[Test]
		public void TestMap_NotNormalized()
		{
			Graph.SinMap1DNode map1DNode = new Graph.SinMap1DNode(false, 0);

			SomeMap<float, float> map = map1DNode.ValueForOutput<SomeMap<float, float>>("value", null, null) as SomeMap<float, float>;
			Assert.NotNull(map);

			Assert.AreEqual(Mathf.Sin(0), map.ValueFor(0), .001f);
			Assert.AreEqual(Mathf.Sin(0.3f * 2.0f * Mathf.PI), map.ValueFor(0.3f), .001f);
		}

		[Test]
		public void TestMap_Normalized()
		{
			Graph.SinMap1DNode map1DNode = new Graph.SinMap1DNode(true, 0);

			SomeMap<float, float> map = map1DNode.ValueForOutput<SomeMap<float, float>>("value", null, null) as SomeMap<float, float>;
			Assert.NotNull(map);

			Assert.AreEqual(0.5f, map.ValueFor(0), .001f);
			Assert.AreEqual(1.0f, map.ValueFor(0.25f), .001f);
			Assert.AreEqual(0, map.ValueFor(0.75f), .001f);
			Assert.AreEqual(0.5f, map.ValueFor(1.0f), .001f);
		}
	}
}
