using UnityEditor;
using UnityEngine;
using System.Collections;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Build a PolyFrame mesh
    /// </summary>
    // FUTURE: add support for world size to scale polygon
    public class PolyFrameMeshBuilder : SomeMeshBuilder
    {
        public bool isClosed = false;
        public float strokeWidth = 0.1f;
        public Polygon polygon = new Polygon();

        public override Vector2 WorldSize
        {
            get => polygon.Size;
            set
            {
                Debug.Log("World size for poly frame not yet supported");
            }
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new PolyFrameMesh(polygon, isClosed, strokeWidth);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }
    }
}
