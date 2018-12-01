using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovePath2D : MonoBehaviour
{

	private enum RenderState
	{
		Default,
		Selected
	}

	public bool isLooping = false;
	public List<GameObject> waypoints = new List<GameObject>();

	// Use this for initialization
	void Start()
	{

	}

	// Update is called once per frame
	void Update()
	{

	}

	void OnValidate()
	{
	}

	public List<Vector3> GetWaypointPositions()
	{
		if (waypoints.Count == 0) { return new List<Vector3>(); }

		List<Vector3> positions = new List<Vector3>();

		foreach (GameObject waypoint in waypoints)
		{
			positions.Add(waypoint.transform.position);
		}
		if (isLooping)
		{
			positions.Add(waypoints[0].transform.position);
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

	private void RenderGizmos(RenderState renderState)
	{
		if (waypoints.Count == 0) { return; }

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
					case RenderState.Default:
						Gizmos.color = Color.white;
						break;
					case RenderState.Selected:
						Gizmos.color = GUI.skin.settings.selectionColor;
						break;
				}
				Gizmos.DrawLine(positionStart, positionEnd);
			}
			hasFirstPosition = true;
			prevPosition = position;
		}
	}

	void OnDrawGizmos()
	{
		RenderGizmos(RenderState.Default);
	}

	private void OnDrawGizmosSelected()
	{
		RenderGizmos(RenderState.Selected);
	}
}
