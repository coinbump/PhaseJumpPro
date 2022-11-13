using System;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// A view for rendering a 2D shape
	/// </summary>
	public class Shape2D : View2D
	{
		public Material strokeMaterial;
		public Material fillMaterial;

        public Shape2D()
		{
		}

        /// <summary>
        /// Shapes are composed of multiple parts (frame, fill, etc.)
        /// Build a part and add it as a child object
        /// </summary>
		public GameObject AddPartObject<MeshBuilder>(Material material, float z = 0) where MeshBuilder : SomeMeshBuilder
		{
            if (!material) { return null; }

            var partObject = new GameObject();
            partObject.transform.parent = gameObject.transform;

            var meshRenderer = partObject.AddComponent<MeshRenderer>();
            meshRenderer.material = material;

            var meshBuilder = partObject.AddComponent<MeshBuilder>();

            meshBuilder.WorldSize = frame.size;
            meshBuilder.Build();

            partObject.transform.localPosition = new Vector3(0, 0, z);

            return partObject;
        }
    }
}
