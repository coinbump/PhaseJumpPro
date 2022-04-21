using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/15/22
 */
namespace PJ
{
    /// <summary>
    /// Build a UV scroller mesh with two separated quads
    /// whose UV coordinates can be altered for endless scrolling of a repeating texture
    /// </summary>
    public class UVScrollerMeshBuilder : SomeMeshBuilder
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public float scrollValue = 0;

        public override Vector2 WorldSize
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var uvScrollerMesh = new UVScrollerMesh(worldSize, scrollValue);
            var mesh = uvScrollerMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(UVScrollerMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    UVScrollerMeshBuilder meshBuilder = (UVScrollerMeshBuilder)target;
                    meshBuilder.Build();
                }
            }
        }
#endif
    }
}
