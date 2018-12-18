using UnityEditor;
using UnityEngine;

namespace PJ
{
	public static class GroupObjectsCommand
	{
		[MenuItem("Phase Jump/Objects/Group Selected %g")]
		private static void GroupSelected()
		{
			if (!Selection.activeTransform) return;

			var gameObject = new GameObject(Selection.activeTransform.name + " Group");
			Undo.RegisterCreatedObjectUndo(gameObject, "Group Selected");
			gameObject.transform.SetParent(Selection.activeTransform.parent, false);
			foreach (var transform in Selection.transforms)
			{
				Undo.SetTransformParent(transform, gameObject.transform, "Group Selected");
			}
			Selection.activeGameObject = gameObject;
		}
	}
}
