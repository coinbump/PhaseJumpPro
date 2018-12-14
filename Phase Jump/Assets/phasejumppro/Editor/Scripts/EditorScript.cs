#if (UNITY_EDITOR)

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using System.IO;

namespace PJ
{
	public class EditorScriptWindow : EditorWindow
	{
		public List<string> FindPathsInFolder(string searchParams, string folderName)
		{
			List<string> result = new List<string>();

			if (folderName.Length == 0)
			{
				Debug.Log("ERROR. Need a folder name for script.");
				return result;
			}

			string[] guids1 = AssetDatabase.FindAssets(searchParams, null);

			foreach (string guid1 in guids1)
			{
				var path = AssetDatabase.GUIDToAssetPath(guid1);

				string testString = folderName + Path.DirectorySeparatorChar;
				if (path.ToLower().Contains(testString.ToLower()))  // Case insensitive
				{
					result.Add(path);
				}
			}

			return result;
		}

		public void ProcessPaths(List<string> paths)
		{
			if (paths.Count == 0) { return; }

			//EditorProgressBarWindow progressBarWindow = GetWindow<EditorProgressBarWindow>();
			//progressBarWindow.maxValue = paths.Count;
			//progressBarWindow.Show();

			foreach (string path in paths)
			{
				ProcessPath(path);
				//progressBarWindow.value += 1;
			}

			//progressBarWindow.Close();
		}

		public virtual void ProcessPath(string path) { }
	}

}

#endif
