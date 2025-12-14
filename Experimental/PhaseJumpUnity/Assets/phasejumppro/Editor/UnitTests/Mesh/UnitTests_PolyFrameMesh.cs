using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_PolyFrameMesh
    {
        private class Polygon1X : PJ.Polygon
        {
            public Polygon1X()
            {
                Add(new Vector3(0, 0));
                Add(new Vector3(1, 0));
            }
        }

        private class Polygon2X : PJ.Polygon
        {
            public Polygon2X()
            {
                Add(new Vector3(0, 0));
                Add(new Vector3(1, 0));
                Add(new Vector3(1, 1));
            }
        }

        [Test]
        public void Test1xUnclosedPolyFrameMesh()
        {
            var shapeMesh = new PolyFrameMesh(new Polygon1X(), false, 2.0f);
            var mesh = shapeMesh.Build();

            var vertices = mesh.vertices;
            Assert.AreEqual(4, vertices.Length);

            var triangles = mesh.triangles;
            Assert.AreEqual(6, triangles.Length);

            var uv = mesh.uv;
            Assert.AreEqual(vertices.Length, uv.Length);

            Assert.AreEqual(0, triangles[0]);
            Assert.AreEqual(2, triangles[1]);
            Assert.AreEqual(1, triangles[2]);
            Assert.AreEqual(2, triangles[3]);
            Assert.AreEqual(3, triangles[4]);
            Assert.AreEqual(1, triangles[5]);
        }

        [Test]
        public void Test2xUnclosedPolyFrameMesh()
        {
            var shapeMesh = new PolyFrameMesh(new Polygon2X(), false, 2.0f);
            var mesh = shapeMesh.Build();

            var vertices = mesh.vertices;
            Assert.AreEqual(8, vertices.Length);

            var triangles = mesh.triangles;
            Assert.AreEqual(12, triangles.Length);

            var uv = mesh.uv;
            Assert.AreEqual(vertices.Length, uv.Length);

            Assert.AreEqual(0, triangles[0]);
            Assert.AreEqual(2, triangles[1]);
            Assert.AreEqual(1, triangles[2]);
            Assert.AreEqual(2, triangles[3]);
            Assert.AreEqual(3, triangles[4]);
            Assert.AreEqual(1, triangles[5]);

            Assert.AreEqual(4, triangles[6]);
            Assert.AreEqual(6, triangles[7]);
            Assert.AreEqual(5, triangles[8]);
            Assert.AreEqual(6, triangles[9]);
            Assert.AreEqual(7, triangles[10]);
            Assert.AreEqual(5, triangles[11]);
        }

        [Test]
        public void Test2xClosedPolyFrameMesh()
        {
            var shapeMesh = new PolyFrameMesh(new Polygon2X(), true, 2.0f);
            var mesh = shapeMesh.Build();

            var vertices = mesh.vertices;
            Assert.AreEqual(12, vertices.Length);

            var triangles = mesh.triangles;
            Assert.AreEqual(18, triangles.Length);

            var uv = mesh.uv;
            Assert.AreEqual(vertices.Length, uv.Length);

            Assert.AreEqual(0, triangles[0]);
            Assert.AreEqual(2, triangles[1]);
            Assert.AreEqual(1, triangles[2]);
            Assert.AreEqual(2, triangles[3]);
            Assert.AreEqual(3, triangles[4]);
            Assert.AreEqual(1, triangles[5]);

            Assert.AreEqual(4, triangles[6]);
            Assert.AreEqual(6, triangles[7]);
            Assert.AreEqual(5, triangles[8]);
            Assert.AreEqual(6, triangles[9]);
            Assert.AreEqual(7, triangles[10]);
            Assert.AreEqual(5, triangles[11]);

            Assert.AreEqual(8, triangles[12]);
            Assert.AreEqual(10, triangles[13]);
            Assert.AreEqual(9, triangles[14]);
            Assert.AreEqual(10, triangles[15]);
            Assert.AreEqual(11, triangles[16]);
            Assert.AreEqual(9, triangles[17]);
        }
    }
}
