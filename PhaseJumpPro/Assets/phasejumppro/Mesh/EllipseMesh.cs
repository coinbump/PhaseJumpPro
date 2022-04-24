using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/3/22
 */
namespace PJ
{
    /// <summary>
    /// Renders an ellipse
    /// </summary>
    public class EllipseMesh : SomeMesh
    {
        public float angleStep = 10.0f;
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        protected ArcMesh arcMesh;

        public override int MeshVertexCount
        {
            get => arcMesh.MeshVertexCount;
        }

        public EllipseMesh(float angleStep, Vector2 worldSize)
        {
            this.angleStep = angleStep;
            this.worldSize = worldSize;
            arcMesh = new ArcMesh(0, 360.0f, angleStep, worldSize);
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            return arcMesh.BuildMesh(mesh);
        }
    }
}
