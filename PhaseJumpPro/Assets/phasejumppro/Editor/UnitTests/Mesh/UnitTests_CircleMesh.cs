using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
    public class UnitTests_CircleMesh
    {
        [Test]
        public void TestCircleMesh()
        {
            var radius = 2.5f;
            var shapeMesh = new CircleMesh(Angle.DegreesAngle(120.0f), radius);
            var mesh = shapeMesh.Build();

            var vertices = mesh.vertices;
            Assert.AreEqual(5, vertices.Length);

            var triangles = mesh.triangles;
            Assert.AreEqual(3 * 3, triangles.Length);

            var uv = mesh.uv;
            Assert.AreEqual(vertices.Length, uv.Length);

            Assert.AreEqual(0, triangles[0]);
            Assert.AreEqual(1, triangles[1]);
            Assert.AreEqual(2, triangles[2]);
            Assert.AreEqual(0, triangles[3]);
            Assert.AreEqual(2, triangles[4]);
            Assert.AreEqual(3, triangles[5]);
            Assert.AreEqual(0, triangles[6]);
            Assert.AreEqual(3, triangles[7]);
            Assert.AreEqual(4, triangles[8]);

            Assert.AreEqual(new Vector2(0.5f, 0.5f), uv[0]);
            Assert.AreEqual(0.5f, uv[1].x, .001f);
            Assert.AreEqual(1.0f, uv[1].y, .001f);

            Assert.AreEqual(new Vector3(0, 0, 0), vertices[0]);
            Assert.AreEqual(0, vertices[1].x, .001f);
            Assert.AreEqual(radius, vertices[1].y, .001f);
            Assert.AreEqual(0, vertices[1].z, .001f);

            Assert.AreEqual(2.165f, vertices[2].x, .001f);
            Assert.AreEqual(-1.25f, vertices[2].y, .001f);
            Assert.AreEqual(0, vertices[2].z, .001f);

            Assert.AreEqual(-2.165f, vertices[3].x, .001f);
            Assert.AreEqual(-1.25f, vertices[3].y, .001f);
            Assert.AreEqual(0, vertices[3].z, .001f);
        }
    }
}
