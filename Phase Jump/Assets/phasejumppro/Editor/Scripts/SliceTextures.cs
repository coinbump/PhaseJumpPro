﻿#if (UNITY_EDITOR)

using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

/// <summary>
/// Automatically slice textures based on a uniform cell size
/// 
/// RATING: 4 stars. Works. Could use advanced options
/// CODE REVIEW: 12.13.18 
/// </summary>
public class SliceTexture : MonoBehaviour
{
	/// <summary>
	/// Automatically slice textures based on a uniform cell size
	/// </summary>
	[MenuItem("Phase Jump/Slice Textures…")]
	static void Foo()
	{
		EditorWindow.GetWindow<Window>(false, "Slice Textures");
	}

	public class Window : EditorWindow
	{
		public class Parameters
		{
			public string folderName = "spritesheets";
			public int width = 16;
			public int height = 16;
		}

		public void Slice(Parameters parameters)
		{
			string[] guids1 = AssetDatabase.FindAssets("t:texture2D", null);

			foreach (string guid1 in guids1)
			{
				var path = AssetDatabase.GUIDToAssetPath(guid1);

				string testString = parameters.folderName + Path.DirectorySeparatorChar;
				if (path.Contains(testString))
				{
					Debug.Log(path);
					Texture2D texture = (Texture2D)AssetDatabase.LoadAssetAtPath<Texture2D>(path);

					TextureImporter importer = (TextureImporter)AssetImporter.GetAtPath(path);

					// Bug Workaround: we can't reslice an already sliced texture,
					// Force it to single to allow slicing.
					if (SpriteImportMode.Multiple == importer.spriteImportMode)
					{
						importer.spriteImportMode = SpriteImportMode.Single;
						AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate);
					}

					importer.spriteImportMode = SpriteImportMode.Multiple;
					importer.isReadable = true;	// So we can read the pixels.

					List<SpriteMetaData> spriteMetadatas = new List<SpriteMetaData>();
					int sliceWidth = parameters.width;
					int sliceHeight = parameters.height;

					for (int i = 0; i < texture.width; i += sliceWidth)
					{
						for (int j = texture.height; j > 0; j -= sliceHeight)
						{
							SpriteMetaData smd = new SpriteMetaData
							{
								pivot = new Vector2(0.5f, 0.5f),
								alignment = (int)SpriteAlignment.Custom,

								name = texture.name + "_" + (i / sliceWidth) + "_" + (texture.height - j) / sliceHeight
							};

							if (texture.height == sliceHeight)
							{
								// Single row, use name_N instead of name_N_N (cleaner)
								smd.name = texture.name + "_" + (i / sliceWidth);
							}
							smd.rect = new Rect(i, j - sliceHeight, sliceWidth, sliceHeight);

							bool gotAnyAlpha = false;
							Color[] pixels = texture.GetPixels((int)smd.rect.xMin, (int)smd.rect.yMin, sliceWidth, sliceHeight, 0);
							foreach (Color color in pixels)
							{
								if (color.a > 0)
								{
									gotAnyAlpha = true;
									break;
								}
							}

							if (gotAnyAlpha)
							{
								spriteMetadatas.Add(smd);
							}
							else
							{
								Debug.Log(string.Format("No alpha at {0}, {1}", i, j));
							}
						}
					}

					if (spriteMetadatas.Count > 1)
					{
						importer.spritesheet = spriteMetadatas.ToArray();
					}
					else
					{
						importer.spritesheet = new List<SpriteMetaData>().ToArray();
						importer.spriteImportMode = SpriteImportMode.Single;
					}
					AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate);
				}
				else
				{
					//Debug.Log(string.Format("SKIPPED {0}", path));
				}
			}

		}

		void OnGUI()
		{
			BuildWindow();
		}

		string folderName = "spritesheets";
		string widthString = "16";
		string heightString = "16";

		void BuildWindow()
		{
			GUILayout.Label("Folder Name");
			folderName = GUILayout.TextField(folderName);

			GUILayout.Label("Width");
			widthString = GUILayout.TextField(widthString, 5);

			GUILayout.Label("Height");
			heightString = GUILayout.TextField(heightString, 5);

			if (GUILayout.Button("Slice"))
			{
				Parameters parameters = new Parameters
				{
					folderName = folderName,
					width = Convert.ToInt32(widthString),
					height = Convert.ToInt32(heightString)
				};

				Slice(parameters);
			}
		}
	}
}

#endif

