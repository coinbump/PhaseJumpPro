using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	[ExecuteInEditMode]	// Works for class, not methods
	public abstract class AbstractMovePath2D : MonoBehaviour
	{
		public List<GameObject> targets;

		public abstract void SnapNodeToPath(Node2D node, bool force);
		public abstract void UpdateNodeOnPath(Node2D node);	// Called from Update or FixedUpdate (but not both!)

		protected virtual void OnValidate()
		{
			foreach (GameObject target in targets)
			{
				Node2D node = target.GetComponent<Node2D>();
				if (null != node)
				{
					node.pathInfo.path = this.gameObject;
					node.SnapToPath(true);
				}
			}
		}

		protected virtual void Update()
		{
#if (UNITY_EDITOR)
			if (transform.hasChanged)
			{
				OnValidate();
				transform.hasChanged = false;
			}
#endif
		}
	}
}
