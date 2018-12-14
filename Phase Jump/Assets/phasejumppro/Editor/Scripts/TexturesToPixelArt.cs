#if (UNITY_EDITOR)

using System.IO;
using UnityEngine;
using UnityEditor;

/// <summary>
/// Applies proper settings to textures for pixel art (point filter, no compression)
/// </summary>
/// <remarks>
/// RATING: 5 stars. Simple, effective. Does what it needs to do.
/// CODE REVIEW: 12.13.18 
/// 
/// FUTURE: maybe add path options? Only change images at specific paths in Assets?
/// </remarks>
public class SetupSpriteSheet
{
	[MenuItem("Phase Jump/Textures -> Pixel Art…")]
	static void Foo()
	{
		EditorWindow.GetWindow<Window>(false, "Textures -> Pixel Art");
	}

	public class Window : EditorWindow
	{
		public class Parameters
		{
			public string folderName = "spritesheets";
		}
	
		void RunScript(Parameters parameters)
		{
			string[] guids1 = AssetDatabase.FindAssets("t:texture2D", null);

			foreach (string guid1 in guids1)
			{
				Debug.Log(AssetDatabase.GUIDToAssetPath(guid1));
				var path = AssetDatabase.GUIDToAssetPath(guid1);

				string testString = parameters.folderName + Path.DirectorySeparatorChar;
				if (path.Contains(testString)) { 

					TextureImporter importer = (TextureImporter)AssetImporter.GetAtPath(path);
					importer.filterMode = FilterMode.Point;
					importer.textureCompression = TextureImporterCompression.Uncompressed;
				}
			}
		}

		void OnGUI()
		{
			BuildWindow();
		}

		string folderName = "spritesheets";

		void BuildWindow()
		{
			GUILayout.Label("Folder Name");
			folderName = GUILayout.TextField(folderName);

			if (GUILayout.Button("Apply"))
			{
				Parameters parameters = new Parameters
				{
					folderName = folderName,
				};

				RunScript(parameters);
			}
		}
	}
}

#endif