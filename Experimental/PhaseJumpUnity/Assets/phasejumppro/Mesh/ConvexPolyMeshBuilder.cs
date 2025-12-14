using UnityEditor;
using UnityEngine;
using System.Collections;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Build a Convex poly mesh
    /// </summary>
    // FUTURE: add support for world size to scale polygon
    public class ConvexPolyMeshBuilder : SomeMeshBuilder
    {
        public Polygon polygon = new Polygon();

        protected override Vector2 WorldSizeInternal
        {
            get => polygon.Size;
            set
            {
                Debug.Log("World size for convex poly not yet supported");
            }
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new ConvexPolyMesh(polygon);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }
    }
}
