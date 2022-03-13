using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_PlaneMesh
	{
		[Test]
		public void Test1x1PlaneMesh()
		{
			var planeMesh = new PlaneMesh(new Vector2Int(1, 1), new Vector3(2.0f, 2.0f));
			var mesh = planeMesh.Build();

			var vertices = mesh.vertices;
			Assert.AreEqual(4, vertices.Length);

			Assert.AreEqual(2, planeMesh.VerticesSize.x);
			Assert.AreEqual(2, planeMesh.VerticesSize.y);

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

			Assert.AreEqual(new Vector2(0, 0), uv[0]);
			Assert.AreEqual(new Vector2(1, 0), uv[1]);
			Assert.AreEqual(new Vector2(0, 1), uv[2]);
			Assert.AreEqual(new Vector2(1, 1), uv[3]);

            Assert.AreEqual(new Vector3(-1.0f, 0, -1.0f), vertices[0]);
            Assert.AreEqual(new Vector3(1.0f, 0, -1.0f), vertices[1]);
            Assert.AreEqual(new Vector3(-1.0f, 0, 1.0f), vertices[2]);
            Assert.AreEqual(new Vector3(1.0f, 0, 1.0f), vertices[3]);
		}

		[Test]
		public void Test2x1PlaneMesh()
		{
			var planeMesh = new PlaneMesh(new Vector2Int(2, 1), new Vector3(2.0f, 2.0f));
			var mesh = planeMesh.Build();

			var vertices = mesh.vertices;
			Assert.AreEqual(6, vertices.Length);

			var triangles = mesh.triangles;
			Assert.AreEqual(12, triangles.Length);

			var uv = mesh.uv;
			Assert.AreEqual(vertices.Length, uv.Length);

			Assert.AreEqual(0, triangles[0]);
			Assert.AreEqual(3, triangles[1]);
			Assert.AreEqual(1, triangles[2]);
			Assert.AreEqual(3, triangles[3]);
			Assert.AreEqual(4, triangles[4]);
			Assert.AreEqual(1, triangles[5]);

			Assert.AreEqual(1, triangles[6]);
			Assert.AreEqual(4, triangles[7]);
			Assert.AreEqual(2, triangles[8]);
			Assert.AreEqual(4, triangles[9]);
			Assert.AreEqual(5, triangles[10]);
			Assert.AreEqual(2, triangles[11]);

			Assert.AreEqual(new Vector2(0, 0), uv[0]);
			Assert.AreEqual(new Vector2(0.5f, 0), uv[1]);
			Assert.AreEqual(new Vector2(1.0f, 0), uv[2]);
			Assert.AreEqual(new Vector2(0, 1.0f), uv[3]);
			Assert.AreEqual(new Vector2(0.5f, 1.0f), uv[4]);
			Assert.AreEqual(new Vector2(1.0f, 1.0f), uv[5]);

			Assert.AreEqual(new Vector3(-1.0f, 0, -1.0f), vertices[0]);
			Assert.AreEqual(new Vector3(0f, 0, -1.0f), vertices[1]);
			Assert.AreEqual(new Vector3(1.0f, 0, -1.0f), vertices[2]);
			Assert.AreEqual(new Vector3(-1.0f, 0, 1.0f), vertices[3]);
			Assert.AreEqual(new Vector3(0, 0, 1.0f), vertices[4]);
			Assert.AreEqual(new Vector3(1.0f, 0, 1.0f), vertices[5]);
		}

		[Test]
		public void Test1x2PlaneMesh()
		{
			var planeMesh = new PlaneMesh(new Vector2Int(1, 2), new Vector3(2.0f, 2.0f));
			var mesh = planeMesh.Build();

			var vertices = mesh.vertices;
			Assert.AreEqual(6, vertices.Length);

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

			Assert.AreEqual(2, triangles[6]);
			Assert.AreEqual(4, triangles[7]);
			Assert.AreEqual(3, triangles[8]);
			Assert.AreEqual(4, triangles[9]);
			Assert.AreEqual(5, triangles[10]);
			Assert.AreEqual(3, triangles[11]);

			Assert.AreEqual(new Vector2(0, 0), uv[0]);
			Assert.AreEqual(new Vector2(1.0f, 0), uv[1]);
			Assert.AreEqual(new Vector2(0, 0.5f), uv[2]);
			Assert.AreEqual(new Vector2(1.0f, 0.5f), uv[3]);
			Assert.AreEqual(new Vector2(0, 1.0f), uv[4]);
			Assert.AreEqual(new Vector2(1.0f, 1.0f), uv[5]);

			Assert.AreEqual(new Vector3(-1.0f, 0, -1.0f), vertices[0]);
			Assert.AreEqual(new Vector3(1.0f, 0, -1.0f), vertices[1]);
			Assert.AreEqual(new Vector3(-1.0f, 0, 0), vertices[2]);
			Assert.AreEqual(new Vector3(1.0f, 0, 0f), vertices[3]);
			Assert.AreEqual(new Vector3(-1.0f, 0, 1.0f), vertices[4]);
			Assert.AreEqual(new Vector3(1.0f, 0, 1.0f), vertices[5]);
		}
	}
}
