using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
	public class MovePath2D : SomeMovePath2D
	{
		public bool isLooping = false;

		private List<GameObject> _waypoints = new List<GameObject>();

		public List<GameObject> Waypoints
		{
			get
			{
#if UNITY_EDITOR
				UpdateWaypoints();	
#endif
				return _waypoints;
			}
		}

		public void UpdateWaypoints()
		{
			_waypoints = new List<GameObject>();

			foreach (Transform child in transform)
			{
				_waypoints.Add(child.gameObject);
			}
		}

		private void Awake()
		{
			UpdateWaypoints();
		}

		void Start()
		{

		}

		// Update is called once per frame
		protected override void Update()
		{
			base.Update();
		}

		protected override void OnValidate()
		{
			foreach (GameObject target in targets)
			{
				GoNode2D node = target.GetComponent<GoNode2D>();
				if (null != node)
				{
					node.pathInfo.waypointOrigin = Math.Min(node.pathInfo.waypointOrigin, Waypoints.Count - 1);
					node.pathInfo.waypointOrigin = Math.Max(node.pathInfo.waypointOrigin, 0);
				}
			}

			base.OnValidate();
		}

		public List<Vector3> GetWaypointPositions()
		{
			if (Waypoints.Count == 0) { return new List<Vector3>(); }

			List<Vector3> positions = new List<Vector3>();

			foreach (GameObject waypoint in Waypoints)
			{
				if (waypoint == null)
				{
					Debug.Log("Missing Waypoint GameObject for " + gameObject.name);
					return positions;
				}

				positions.Add(waypoint.transform.position);
			}
			if (isLooping)
			{
				positions.Add(Waypoints[0].transform.position);
			}

			return positions;
		}

		public List<Vector2> GetWaypointPositions2D()
		{
			List<Vector3> result3D = GetWaypointPositions();
			List<Vector2> result = new List<Vector2>();

			foreach (Vector3 v in result3D)
			{
				result.Add(v);
			}

			return result;
		}

#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			if (Waypoints.Count == 0) { return; }

			List<Vector3> positions = GetWaypointPositions();

			bool hasFirstPosition = false;
			Vector3 prevPosition = new Vector3();
			Vector3 positionStart, positionEnd;

			foreach (Vector3 position in positions)
			{
				positionStart = prevPosition;
				positionEnd = position;

				if (hasFirstPosition)
				{
					switch (renderState)
					{
						case EditorUtils.RenderState.Default:
							Gizmos.color = Color.white;
							break;
						case EditorUtils.RenderState.Selected:
							Gizmos.color = GUI.skin.settings.selectionColor;
							break;
					}
					Gizmos.DrawLine(positionStart, positionEnd);
				}
				hasFirstPosition = true;
				prevPosition = position;
			}
		}
#endif

		public override void SnapNodeToPath(GoNode2D node, bool force = false)
		{
			int origin = node.pathInfo.waypointOrigin;
			var positions = GetWaypointPositions2D();

			if (positions.Count < 2) { return; }

			Vector2 positionStart;
			Vector2 positionEnd;
			GetPathPositions(node, out positionStart, out positionEnd);

			Vector2 position = Vector2.Lerp(positionStart, positionEnd, node.pathInfo.waypointProgress);

			node.MoveToPosition(position, force);

			if (node.pathInfo.orientToPath)
			{
				float angle = Mathf.Atan2(positionEnd.x - positionStart.x, positionEnd.y - positionStart.y) * Mathf.Rad2Deg;
				node.RotationDegreeAngle = angle;
			}
		}

		public override void UpdateNodeOnPath(GoNode2D node)
		{
			var positions = this.GetWaypointPositions2D();

			GoNode2D.PathInfo pathInfo = node.pathInfo;

			// TODO: this only works with simple paths, we need better logic here.
			pathInfo.waypointProgress += Time.deltaTime * speed * pathInfo.speed;

			while (pathInfo.waypointProgress >= 1.0f)
			{
				pathInfo.waypointProgress -= 1.0f;

				if (pathInfo.isMovingForward)
				{
					pathInfo.waypointOrigin += 1;

					if (this.isLooping)
					{
						if (pathInfo.waypointOrigin >= positions.Count - 1)
						{
							pathInfo.waypointOrigin = 0;
						}
					}
					else
					{
						if (pathInfo.waypointOrigin >= positions.Count - 1)
						{
							pathInfo.waypointOrigin = this.Waypoints.Count - 1;
							pathInfo.isMovingForward = false;
						}
					}
				}
				else
				{
					pathInfo.waypointOrigin -= 1;
					if (pathInfo.waypointOrigin < 0 && this.isLooping)
					{
						pathInfo.waypointOrigin = this.Waypoints.Count - 1;
					}
					else if (pathInfo.waypointOrigin <= 0 && !this.isLooping)
					{
						pathInfo.waypointOrigin = 0;
						pathInfo.isMovingForward = true;
					}
				}
			}
		}

		protected void GetPathPositions(GoNode2D node, out Vector2 positionStart, out Vector2 positionEnd)
		{
			int origin = node.pathInfo.waypointOrigin;
			var positions = GetWaypointPositions2D();

			positionStart = transform.position;
			positionEnd = transform.position;

			if (positions.Count < 2) { return; }

			positionStart = positions[origin];
			positionEnd = positionStart;

			if (node.pathInfo.isMovingForward)
			{
				positionEnd = positions[origin + 1];
			}
			else
			{
				if (origin > 0)
				{
					positionEnd = positions[origin - 1];
				}
				else if (isLooping)
				{
					positionEnd = positions[positions.Count - 2];
				}
			}
		}
	}
}

