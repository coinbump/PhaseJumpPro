using UnityEngine;
using System.Collections.Generic;
using NUnit.Framework;
using System;

namespace PJ
{
    public class UnitTests_PerlinNoiseNode
    {
        private class Node : PJ.Graph.GoStandardAcyclicNode
        {
        }

        [Test]
        public void TestPerlinNoise()
        {
            var perlinNoiseNode = new Graph.PerlinNoiseNode(Vector2.zero, new Vector2(1.0f, 1.0f));
            var map = perlinNoiseNode.ValueForOutput<SomeTransform<Vector2, float>>("map", null, null);

            Assert.IsTrue(map is Noise2D.Perlin);
            Assert.AreEqual((map as Noise2D.Perlin).ValueFor(Vector2.zero), Mathf.PerlinNoise(0, 0));
        }
    }
}
