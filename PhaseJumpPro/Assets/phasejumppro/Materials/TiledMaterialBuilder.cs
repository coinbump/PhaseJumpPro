using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple utility class
 * CODE REVIEW: 4/7/2
 */
namespace PJ
{
	/// <summary>
    /// Sets up material for tiled rendering
    /// </summary>
	public class TiledMaterialBuilder : WorldComponent
	{
		public Vector2 tiling = new Vector2(1.0f, 1.0f);
		public Vector2 offset = new Vector2(0.0f, 0.0f);

		protected override void Start()
		{
			Build();
		}

		public void Build()
		{
			var meshRenderer = GetComponent<MeshRenderer>();
			if (null == meshRenderer) { return; }

			meshRenderer.material.mainTexture.wrapMode = TextureWrapMode.Repeat;
			meshRenderer.material.mainTextureScale = new Vector2(tiling.x, tiling.y);
			meshRenderer.material.mainTextureOffset = offset;
		}
	}
}
