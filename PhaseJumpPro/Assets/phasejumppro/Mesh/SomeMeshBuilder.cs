using UnityEngine;
using UnityEditor;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/27/22
 */
namespace PJ
{
    /// <summary>
    /// For Mesh builders.
    /// Requires a MeshFilter component
    /// </summary>
    [RequireComponent(typeof(MeshFilter))]
    [RequireComponent(typeof(MeshRenderer))]
    public abstract class SomeMeshBuilder : WorldComponent
    {
        // Use this for initialization
        protected override void Start()
        {
            base.Start();

            Build();
        }

        public virtual void Build()
        {
            var meshFilter = GetComponent<MeshFilter>();
            if (null == meshFilter)
            {
                meshFilter = gameObject.AddComponent<MeshFilter>();
            }

            meshFilter.mesh = BuildMesh();

            var meshCollider = GetComponent<MeshCollider>();
            if (null != meshCollider)
            {
                meshCollider.sharedMesh = meshFilter.mesh;
            }
        }

        public abstract Mesh BuildMesh();
        public abstract Vector2 WorldSize { get; set; }

#if UNITY_EDITOR
        [CustomEditor(typeof(SomeMeshBuilder), true)]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    SomeMeshBuilder someMeshBuilder = (SomeMeshBuilder)target;
                    someMeshBuilder.Build();
                }
            }
        }

        protected virtual void OnValidate()
        {
            if (TryGetComponent(out MeshFilter meshFilter))
            {
                meshFilter.mesh = BuildMesh();
            }
        }
#endif
    }
}
