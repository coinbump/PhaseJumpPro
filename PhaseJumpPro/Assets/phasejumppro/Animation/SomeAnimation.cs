using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple base class
 * CODE REVIEW: 4.20/22
 */
namespace PJ
{
	public abstract class SomeAnimation : MonoBehaviour
	{
		public MeshMaterialType meshMaterialType = MeshMaterialType.Shared;

		protected MultiRenderer multiRenderer;

		protected virtual void Awake()
		{
			Setup();
		}

		protected virtual void Setup()
        {
			if (null != multiRenderer) { return; }
			multiRenderer = new MultiRenderer(gameObject);
			multiRenderer.meshMaterialType = meshMaterialType;
		}

		protected virtual void Start()
        {
        }

		protected virtual void Update()
		{
		}

		protected virtual void OnValidate()
        {
			Setup();
        }
	}
}
