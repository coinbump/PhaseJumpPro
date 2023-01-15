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
    /// Build a plane mesh of NxN size and replace the current mesh
    /// </summary>
    /// Reference: https://docs.unity3d.com/Manual/Example-CreatingaBillboardPlane.html
    public class PlaneMeshBuilder : SomeMeshBuilder
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Axis faceAxis = Axis.Z;

        protected override Vector2 WorldSizeInternal
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var planeMesh = new PlaneMesh(meshSize, worldSize, faceAxis);
            var mesh = planeMesh.Build(true);
            return mesh;
        }
    }
}
