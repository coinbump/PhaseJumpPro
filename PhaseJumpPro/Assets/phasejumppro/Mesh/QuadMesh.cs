using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/28/22
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

        protected PlaneMesh PlaneMesh => new PlaneMesh(meshSize, worldSize, Axis.Y);

        public QuadMesh(Vector2Int meshSize, Vector2 worldSize)
        {
            this.meshSize = meshSize;
            this.worldSize = worldSize;
        }

        public override Mesh BuildMesh()
        {
            return PlaneMesh.BuildMesh();
        }
    }
}
