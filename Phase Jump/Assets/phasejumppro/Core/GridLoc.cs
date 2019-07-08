using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Lets us attach the exact grid location of an object as a component.
	/// </summary>
	public class GridLoc : MonoBehaviour
	{
		public Vector3Int value;
		public Vector3 offset;

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
