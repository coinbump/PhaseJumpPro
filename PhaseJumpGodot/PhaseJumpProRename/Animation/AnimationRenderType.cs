using System.Collections;
using System.Collections.Generic;
using Godot;

namespace PJ
{
	public enum AnimationRenderType
	{
		// Alters SpriteRenderer
		Sprite,

		// Alters Mesh, creating new material
		MeshNewMaterial,

		// Alters Mesh's shared material (changes all instances of that mesh)
		MeshSharedMaterial
	}

	public enum MeshMaterialType
	{
		// Alters Mesh's shared material (changes all instances of that mesh)
		Shared,

		// Alters Mesh, creating new material (CAREFUL: lowers performance)
		Copy
	}
}
