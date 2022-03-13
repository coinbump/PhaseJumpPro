using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 3/13/22
 */
namespace PJ
{
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
                Debug.Log("WARNING. MeshBuilder requires a MeshFilter");
                return;
            }

            meshFilter.mesh = BuildMesh();
        }

        public abstract Mesh BuildMesh();
    }
}
