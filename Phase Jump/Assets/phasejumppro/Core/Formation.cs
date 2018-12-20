using System;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars.
 * CODE REVIEW: 12/19/18
 */
namespace PJ
{
	/// <summary>
	/// Each child object specifies a position to populate a template game object
	/// in the formation.
	/// 
	/// EXAMPLE: line of hearts to show player health (N hearts)
	/// EXAMPLE: V formation of enemies approaching
	/// </summary>
	public class Formation : MonoBehaviour
	{
		public List<GameObject> templates;  // Template objects to populate

		protected ObservedValue<int> autoValue;

		// When observed value changes, we auto populate the formation
		// EXAMPLE: player health shows formation of hearts
		public ObservedValue<int> AutoValue
		{
			set
			{
				autoValue = value;
				void _action(int _value) { PopulateToCount(_value, true); }

				autoValue.AddAction(_action);
			}
		}

		protected List<GameObject> _childObjects = new List<GameObject>();

		// List of child objects (each child is a populate location)
		public List<GameObject> ChildObjects
		{
			get
			{
#if UNITY_EDITOR
				UpdateChildObjects();
#endif

				return _childObjects;
			}
		}

		public List<GameObject> FormationList
		{
			get
			{
				List<GameObject> result = new List<GameObject>();
				foreach (Transform child in transform)
				{
					foreach (Transform ct in child)
					{
						result.Add(ct.gameObject);
					}
				}

				return result;
			}
		}

		protected void UpdateChildObjects()
		{
			_childObjects = new List<GameObject>();

			foreach (Transform child in transform)
			{
				_childObjects.Add(child.gameObject);
			}
		}

		private void Awake()
		{
			UpdateChildObjects();
		}

		GameObject NextTemplate()
		{
			return templates.Count > 0 ? templates[Mathf.RoundToInt(UnityEngine.Random.Range(0, templates.Count-1))] : null;
		}

		public void PopulateAll()
		{
			PopulateToCount(ChildObjects.Count, true);
		}

		/// <summary>
		/// Fill out the formation with up to N template objects
		/// </summary>
		/// <param name="count">N.</param>
		public void PopulateToCount(int count, bool removeExtra)
		{
			var formation = FormationList;

			if (formation.Count == count)
			{
				return;
			}
			else if (formation.Count > count)
			{
				if (removeExtra)
				{
					while (formation.Count > count)
					{
						GameObject go = formation[formation.Count - 1];
						UnityEngine.Object.Destroy(go);
						formation.Remove(go);
						return;
					}
				}
				else
				{
					return;
				}
			}

			foreach (Transform child in transform)
			{
				if (child.transform.childCount > 0)
				{
					continue;
				}

				GameObject template = NextTemplate();
				if (null == template)
				{
					Debug.Log("WARNING. Missing template for formation");
					return;
				}

				GameObject newObject = Instantiate(template, child.position, Quaternion.identity);

				newObject.transform.SetParent(child);
				formation.Add(newObject);

				if (formation.Count >= count)
				{
					break;
				}
			}
		}

		public void DestroyAll()
		{
			var formation = FormationList;

			foreach (GameObject go in formation)
			{
#if UNITY_EDITOR
					UnityEngine.Object.DestroyImmediate(go);
#else
					UnityEngine.Object.Destroy(go);
#endif
			}
			formation.Clear();
		}

		protected void OnValidate()
		{
		}

#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			EditorUtils.RenderLinesBetweenObjects(ChildObjects, renderState);
		}
	}

	[CustomEditor(typeof(Formation))]
	public class FormationEditor : Editor
	{
		public override void OnInspectorGUI()
		{
			DrawDefaultInspector();

			Formation formation = (Formation)target;
			if (GUILayout.Button("Populate"))
			{
				formation.PopulateAll();
			}

			if (GUILayout.Button("Remove All"))
			{
				formation.DestroyAll();
			}
		}
	}
#endif

}

