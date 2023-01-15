using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
    public class UnitTests_CircleMap2DNode
    {
        private class Node : PJ.Graph.GoStandardAcyclicNode
        {
        }

        [Test]
        public void TestMap()
        {
            Graph.CircleMap2DNode mapNode = new Graph.CircleMap2DNode(Vector2.zero, 1.0f, 1.0f, 0);

            SomeTransform<Vector2, float> map = mapNode.ValueForOutput<SomeTransform<Vector2, float>>("value", null, null) as SomeTransform<Vector2, float>;
            Assert.NotNull(map);

            Assert.AreEqual(1.0f, map[Vector2.zero]);
            Assert.AreEqual(1.0f, map[new Vector2(1.0f, 0)]);
            Assert.AreEqual(1.0f, map[new Vector2(-1.0f, 0)]);
            Assert.AreEqual(1.0f, map[new Vector2(0, 1.0f)]);
            Assert.AreEqual(1.0f, map[new Vector2(0, -1.0f)]);

            Assert.AreEqual(0, map[new Vector2(1.01f, 0)]);
            Assert.AreEqual(0, map[new Vector2(-1.01f, 0)]);
            Assert.AreEqual(0, map[new Vector2(0, 1.01f)]);
            Assert.AreEqual(0, map[new Vector2(0, -1.01f)]);
        }
    }
}
