#if UNITY_EDITOR

using System.Text;
using System.Xml;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using PJ;
using System;

namespace PJ
{
	/// <summary>
	/// Automatically slice textures based on an XML file
	/// </summary>
	/// <remarks>
	///
	/// COORDINATES: this uses reading coordinates (top-left of image is 0,0).
	/// FUTURE: support Cartesian coordinates if needed.
	/// </remarks>
	/*
		XML FORMAT:
		<tex_atlas>
		REPEATING:
		<art x="1" y="1" width="200" height="200" name="name">
		</art>
		</tex_atlas>
		*/
	public class SliceTexturesFromXML : MonoBehaviour
	{
		/// <summary>
		/// Automatically slice textures based on a uniform cell size
		/// </summary>
		[MenuItem("Phase Jump/XML Slicer…")]
		static void Foo()
		{
			EditorWindow.GetWindow<Window>(false, "XML Slicer");
		}

		public class Window : EditorScriptWindow
		{
			public class Parameters
			{
				public string folderName = "spritesheets";
				public bool isPixelArt = false;
			}

			public void Slice(Parameters parameters)
			{
				this.parameters = parameters;

				List<string> paths = FindPathsInFolder("t:texture2D", parameters.folderName);
				ProcessPaths(paths);
			}

			public override void ProcessPath(string path)
			{
				Debug.Log(path);

				Texture2D texture = AssetDatabase.LoadAssetAtPath<Texture2D>(path);

				TextureImporter importer = (TextureImporter)AssetImporter.GetAtPath(path);

				if (parameters.isPixelArt)
				{
					importer.filterMode = FilterMode.Point;
					importer.textureCompression = TextureImporterCompression.Uncompressed;
				}

				// TODO: is this fixed in Unity yet?
				// Bug Workaround: we can't reslice an already sliced texture,
				// Force it to single to allow slicing.
				if (SpriteImportMode.Multiple == importer.spriteImportMode)
				{
					importer.spriteImportMode = SpriteImportMode.Single;
					AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate);
				}

				string xmlPath = Path.Combine(Path.GetDirectoryName(path), Path.GetFileNameWithoutExtension(path) + ".xml");
				if (!File.Exists(xmlPath))
				{
					return;
				}

				importer.spriteImportMode = SpriteImportMode.Multiple;

				bool wasReadable = importer.isReadable;
				importer.isReadable = true; // So we can read the pixels.
				AssetDatabase.ImportAsset(path);    // Required to update for isReadable

				List<SpriteMetaData> spriteMetadatas = new List<SpriteMetaData>();

				XmlDocument xmlDoc = new XmlDocument();
				xmlDoc.Load(xmlPath);
				XmlNodeList textureAtlasNodes = xmlDoc.GetElementsByTagName("tex_atlas");
				foreach (XmlNode textureAtlasNode in textureAtlasNodes)
				{
					XmlNodeList artNodes = textureAtlasNode.ChildNodes;

					foreach (XmlNode artNode in artNodes)
					{
						if (artNode.Name != "art")
						{
							continue;
						}

						XmlAttributeCollection attributes = artNode.Attributes;
						int x = Convert.ToInt32(attributes.GetNamedItem("x").InnerXml);
						int y = Convert.ToInt32(attributes.GetNamedItem("y").InnerXml);
						int width = Convert.ToInt32(attributes.GetNamedItem("width").InnerXml);
						int height = Convert.ToInt32(attributes.GetNamedItem("height").InnerXml);
						string artName = attributes.GetNamedItem("name").InnerXml;

						SpriteMetaData smd = new SpriteMetaData
						{
							pivot = new Vector2(0.5f, 0.5f),
							alignment = (int)SpriteAlignment.Custom,

							name = artName,
							rect = new Rect(x, texture.height - (y + height), width, height)
						};
						spriteMetadatas.Add(smd);
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
				importer.isReadable = wasReadable;
				AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate);
			}

			void OnGUI()
			{
				BuildWindow();
			}

			string folderName = "spritesheets";
			bool isPixelArt = false;
			private Parameters parameters;

			void BuildWindow()
			{
				GUILayout.Label("Folder Name");
				folderName = GUILayout.TextField(folderName);

				isPixelArt = GUILayout.Toggle(isPixelArt, "Pixel Art");

				if (GUILayout.Button("Slice"))
				{
					Parameters runParams = new Parameters
					{
						folderName = folderName,
						isPixelArt = isPixelArt
					};

					Slice(runParams);
				}
			}
		}
	}
}

#endif

