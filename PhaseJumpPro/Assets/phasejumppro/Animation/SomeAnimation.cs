using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple base class
 * CODE REVIEW: 4.20/22
 */
namespace PJ
{
	public abstract class SomeAnimation : WorldComponent
	{
		public MeshMaterialType meshMaterialType = MeshMaterialType.Shared;

		protected MultiRenderer multiRenderer;

		protected override void Awake()
		{
			Setup();
		}

		protected virtual void Setup()
        {
			if (null != multiRenderer) { return; }
			multiRenderer = new MultiRenderer(gameObject);
			multiRenderer.meshMaterialType = meshMaterialType;
		}

		protected override void Start()
        {
        }

		protected override void Update()
		{
		}

		protected virtual void OnValidate()
        {
			Setup();
        }
	}
}
