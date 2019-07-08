using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// TODO: Work in progress
namespace PJ
{
	public class GridLoc : MonoBehaviour
	{
		public float x;
		public float y;
		public float z;
	}

	/// <summary>
	/// Unlike Unity's Grid, ours automatically places objects in the correct position
	/// when the grid is resized.
	/// </summary>
	public class Grid2D : MonoBehaviour
	{
		// Start is called before the first frame update
		void Start()
		{

		}

		// Update is called once per frame
		void Update()
		{

		}

		void OnTransformChildrenChanged()
		{
			foreach (Transform child in transform)
			{
				var go = child.gameObject;
				var gridLoc = go.GetComponent<GridLoc>();
				if (null == gridLoc)
				{
					gridLoc = go.AddComponent<GridLoc>();
				}


			}
		}
	}
}
