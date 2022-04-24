using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 3/13/22
 */
namespace PJ
{
    /// <summary>
    /// For Mesh builders.
    /// Requires a MeshFilter component
    /// </summary>
    [RequireComponent(typeof(MeshFilter))]
    [RequireComponent(typeof(MeshRenderer))]
    public abstract class SomeMeshBuilder : PJ.MonoBehaviour
	{
		// Use this for initialization
		void Start()
		{
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
    }
}
