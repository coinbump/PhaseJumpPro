using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_QuadFrameMesh
	{
		[Test]
		public void TestQuadFrameMesh()
		{
			var quadFrameMesh = new QuadFrameMesh(new Vector2(2.0f, 2.0f), new Vector2(1.0f, 1.0f));
			var mesh = quadFrameMesh.Build();

			var vertices = mesh.vertices;
			Assert.AreEqual(8, vertices.Length);

			var triangles = mesh.triangles;
			Assert.AreEqual(8 * 3, triangles.Length);

			var uv = mesh.uv;
			Assert.AreEqual(vertices.Length, uv.Length);

			Assert.AreEqual(0, triangles[0]);
			Assert.AreEqual(5, triangles[1]);
			Assert.AreEqual(4, triangles[2]);
			Assert.AreEqual(0, triangles[3]);
			Assert.AreEqual(1, triangles[4]);
			Assert.AreEqual(5, triangles[5]);

			Assert.AreEqual(3, triangles[18]);
			Assert.AreEqual(4, triangles[19]);
			Assert.AreEqual(7, triangles[20]);
			Assert.AreEqual(3, triangles[21]);
			Assert.AreEqual(0, triangles[22]);
			Assert.AreEqual(4, triangles[23]);

            Assert.AreEqual(new Vector2(0, 1), uv[0]);
            Assert.AreEqual(new Vector2(1, 1), uv[1]);
            Assert.AreEqual(new Vector2(1, 0), uv[2]);
            Assert.AreEqual(new Vector2(0, 0), uv[3]);

            Assert.AreEqual(new Vector3(-1.0f, 1.0f, 0), vertices[0]);
			Assert.AreEqual(new Vector3(1.0f, 1.0f, 0), vertices[1]);
			Assert.AreEqual(new Vector3(1.0f, -1.0f, 0), vertices[2]);
			Assert.AreEqual(new Vector3(-1.0f, -1.0f, 0), vertices[3]);
        }
    }
}
