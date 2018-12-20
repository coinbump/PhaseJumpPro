using UnityEditor;
using System;

namespace PJ
{
	public class EditorProgressBarWindow : EditorWindow
	{
		public float value = 0f;
		public float maxValue = 1.0f;

		static void Init()
		{
			// EXAMPLE:
			//EditorProgressBarWindow progressBarWindow = GetWindow<EditorProgressBarWindow>();
			//progressBarWindow.Show();
		}

		void OnGUI()
		{
			if (maxValue <= 0) { return; }
			float progress = Math.Min(1.0f, value / maxValue);

			// CRASHES in Unity 2018 (will be fixed in 2019).
			//if (progress >= 1.0f)
			//	EditorUtility.ClearProgressBar();
			//else
			//EditorUtility.DisplayProgressBar("Progress", "Working on it...", progress);
		}

		void OnInspectorUpdate()
		{
			Repaint();
		}
	}
}
