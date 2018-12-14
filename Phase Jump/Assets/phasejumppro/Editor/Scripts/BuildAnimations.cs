#if (UNITY_EDITOR)

using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

/*
 * RATING: 4 stars. Simple. Works
 *
 * CODE REVIEW: 12.13.18
 * 
 * FUTURE: Support more parameters, workflow
 */
public class BuildAnimations : MonoBehaviour
{
	/// <summary>
	/// Builds animations from spritesheets (sprites need to be sliced first)
	/// </summary>
	[MenuItem("Phase Jump/Textures -> Animations…")]
	static void Foo()
	{
		EditorWindow.GetWindow<Window>(false, "Build Animations");
	}

	public class Window : EditorWindow
	{
		void BuildAnimations(Parameters parameters) {
			string[] guids1 = AssetDatabase.FindAssets("t:texture2D", null);

			foreach (string guid1 in guids1)
			{
				var path = AssetDatabase.GUIDToAssetPath(guid1);

				string testString = parameters.folderName + Path.DirectorySeparatorChar;
				if (path.Contains(testString))
				{
					Debug.Log(path);

					List<Sprite> sprites = new List<Sprite>();

					string outputName = "Animation";

					UnityEngine.Object[] assets = AssetDatabase.LoadAllAssetsAtPath(path);
					foreach (UnityEngine.Object asset in assets)
					{
						Texture2D texture = asset as Texture2D;
						if (null != texture)
						{
							outputName = texture.name;
						}

						Sprite sprite = asset as Sprite;
						if (null == sprite) { continue; }
						sprites.Add(sprite);
					}
					if (sprites.Count == 0) { continue; }

					string outputPath = "assets" + Path.DirectorySeparatorChar + parameters.destinationPath + Path.DirectorySeparatorChar + outputName + ".anim";
					AnimationClip animationClip = AssetDatabase.LoadAssetAtPath<AnimationClip>(outputPath);
					if (null == animationClip) { 
						animationClip = new AnimationClip();
						AssetDatabase.CreateAsset(animationClip, outputPath);
					}
					else
					{
						Debug.Log(String.Format("Modifying {0}", outputPath));
					}

					animationClip.frameRate = parameters.fps;

					AnimationClipSettings settings = AnimationUtility.GetAnimationClipSettings(animationClip);
					settings.loopTime = true;
					AnimationUtility.SetAnimationClipSettings(animationClip, settings);

					EditorCurveBinding binding = new EditorCurveBinding
					{
						type = typeof(SpriteRenderer),
						path = "",
						propertyName = "m_Sprite"
					};

					ObjectReferenceKeyframe[] spriteKeyFrames = new ObjectReferenceKeyframe[sprites.Count];

					int i = 0;
					foreach (Sprite sprite in sprites)
					{
						spriteKeyFrames[i] = new ObjectReferenceKeyframe
						{
							time = i * (1.0f / parameters.fps),
							value = sprites[i]
						};
						i += 1;
					}

					AnimationUtility.SetObjectReferenceCurve(animationClip, binding, spriteKeyFrames);
					AssetDatabase.ImportAsset(outputPath, ImportAssetOptions.ForceUpdate);
				}
			}
		}

		public class Parameters
		{
			public int fps = 12;
			public string folderName = "source";
			public string destinationPath = "output";
		}

		void OnGUI()
		{
			BuildWindow();
		}

		string fpsString = "12";
		string folderName = "spritesheets";
		string destinationPath = "output";

		void BuildWindow()
		{
			GUILayout.Label("FPS");
			fpsString = GUILayout.TextField(fpsString, 4);

			GUILayout.Label("Folder Name");
			folderName = GUILayout.TextField(folderName);

			GUILayout.Label("Destination Path");
			destinationPath = GUILayout.TextField(destinationPath);

			if (GUILayout.Button("Build Animations"))
			{
				Parameters parameters = new Parameters
				{
					fps = Convert.ToInt32(fpsString),
					folderName = folderName,
					destinationPath = destinationPath
				};

				BuildAnimations(parameters);
			}
		}
	}
}

#endif
