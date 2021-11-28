using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Lets us attach the exact grid location of an object as a component.
	/// </summary>
	public class SnapToGridLoc2D : MonoBehaviour
	{
		public Vector3Int value;	// Location in grid space (z axis is used for layers)
		public Vector3 offset;	// Offset in screen space

		private void OnValidate()
		{
			var parent = transform.parent.gameObject;
			if (null == parent) { return; }

			var grid2D = parent.GetComponent<Grid2D>();
			if (null == grid2D) { return; }

			grid2D.ResizeChild(gameObject);
		}
	}
}
