using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
	public class UnitTests_ClampNode
	{
		private class Node : PJ.Graph.GoStandardAcyclicNode
		{
			public float value = 1.0f;

			public Node(float value)
            {
				this.value = value;
            }

            public override object ValueForOutput<T>(string id, Tags parameters, T defaultValue)
            {
                if (typeof(T).IsAssignableFrom(typeof(float)))
                {
					return value;
                }

				return defaultValue;
            }
        }

		[Test]
		public void TestClampMin()
		{
			Node node = new Node(-40.0f);
			var clampNode = new PJ.Graph.ClampValueNode(0, 1.0f);
			node.AddOutput("output", clampNode, "value");

			var clampedValue = clampNode.ValueForOutput<float>("value", null, -1.0f);
			Assert.AreEqual(clampNode.minValue, clampedValue);
		}

		[Test]
		public void TestClampMax()
		{
			Node node = new Node(40.0f);
			var clampNode = new PJ.Graph.ClampValueNode(0, 1.0f);
			node.AddOutput("output", clampNode, "value");

			var clampedValue = clampNode.ValueForOutput<float>("value", null, -1.0f);
			Assert.AreEqual(clampNode.maxValue, clampedValue);
		}
	}
}
