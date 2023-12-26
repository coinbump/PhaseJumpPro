using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
    public class UnitTests_MultiplyMap2DNode
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

            var factor = 3.0f;
            var multiplyNode = new Graph.MultiplyMap2DNode(factor);
            transformNode.AddOutput("value", multiplyNode, "value");

            SomeTransform<Vector2, float> map = multiplyNode.ValueForOutput<SomeTransform<Vector2, float>>("value", null, null) as SomeTransform<Vector2, float>;
            Assert.NotNull(map);

            Assert.AreEqual(Mathf.Sin(0) * factor, map.ValueFor(new Vector2(0, 0.5f)));
            Assert.AreEqual(Mathf.Sin(0.3f * 2.0f * Mathf.PI) * factor, map.ValueFor(new Vector2(0.3f, 0.5f)), .001f);
        }
    }
}
