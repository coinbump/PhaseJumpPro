using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/12/22
 */
namespace PJ
{
    /// <summary>
    /// A plane mesh that is oriented to xy space, and defaults to 1x1 size
    /// </summary>
    public class QuadMesh : SomeMesh
    {
        public Vector2Int meshSize = new Vector2Int(1, 1);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        protected PlaneMesh planeMesh;

        public override int MeshVertexCount => planeMesh.MeshVertexCount;

        public QuadMesh(Vector2Int meshSize, Vector2 worldSize)
        {
            planeMesh = new PlaneMesh(meshSize, worldSize, Axis.Y);
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            return planeMesh.BuildMesh(mesh);
        }
    }
}
