using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

/*
 * RATING: 5 stars
 * Simple wrapper class
 * CODE REVIEW: 4/18/22
 */
namespace PJ
{
	public interface SomeRenderer
	{
		public Color32 Color { get; set; } 
    }

	/// <summary>
	/// Allows us to write code that will work with multiple types of renderers
	/// without knowing which one is attached to the object
	/// </summary>
	public class MultiRenderer : SomeRenderer
	{
		public MeshMaterialType meshMaterialType = MeshMaterialType.Shared;

		protected SpriteRenderer spriteRenderer;
		protected MeshRenderer meshRenderer;
		protected TextMeshPro textMeshPro;

		public MultiRenderer(GameObject gameObject)
		{
			spriteRenderer = gameObject.GetComponent<SpriteRenderer>();
			meshRenderer = gameObject.GetComponent<MeshRenderer>();
			textMeshPro = gameObject.GetComponent<TextMeshPro>();
		}

		public Renderer Renderer
		{
			get
			{
				if (null != spriteRenderer)
				{
					return spriteRenderer;
				}
				else if (null != textMeshPro)
				{
					return textMeshPro.renderer;
				}
				else if (null != meshRenderer)
				{
					return meshRenderer;
				}
				return null;
			}
        }

		public bool Enabled
        {
			get
            {
				var renderer = Renderer;
				if (null == renderer) { return false; }

				return renderer.enabled;
			}
			set
			{
				var renderer = Renderer;
				if (null == renderer) { return; }

				renderer.enabled = value;
			}
		}

		public Color32 Color
		{
			get
			{
				if (null != spriteRenderer)
				{
					return spriteRenderer.color;
				}
				// Important: The Text mesh uses vertex color, and also has a mesh renderer. Check vertex color before mesh
				else if (null != textMeshPro)
				{
					return textMeshPro.color;
				}
				else if (null != meshRenderer)
				{
					switch (meshMaterialType)
                    {
						case MeshMaterialType.Shared:
							return meshRenderer.sharedMaterial.color;
						default:
							return meshRenderer.material.color;
					}
				}
				return new Color32(255, 255, 255, 255);
			}
			set
			{
				if (null != spriteRenderer)
				{
					spriteRenderer.color = value;
				}
				// Important: The Text mesh uses vertex color, and also has a mesh renderer. Check vertex color before mesh
				else if (null != textMeshPro)
				{
					textMeshPro.color = value;
				}
				else if (null != meshRenderer)
				{
					switch (meshMaterialType)
					{
						case MeshMaterialType.Shared:
							//Debug.Log("Multi Shared Color: " + value.ToString());
							meshRenderer.sharedMaterial.color = value;
							break;
						default:
							meshRenderer.material.color = value;
							break;
					}
				}
			}
		}
	}
}
