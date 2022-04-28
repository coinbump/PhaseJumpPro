using UnityEngine;
using UnityEditor;
using System;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/14/22
 */
namespace PJ
{
    /// <summary>
    /// Sizes a mesh to fit the screen size
    /// </summary>
    public class ScreenMeshSizer2D : MonoBehaviour
    {
        public Camera screenCamera;
        public Vector2 inset = Vector2.zero;
        public Vector2 offset = Vector2.zero;

        public void Awake()
        {
            Build();
        }

        public void Build()
        {
            if (null == screenCamera)
            {
                Debug.Log("Error. Missing camera for screen mesh");
                return;
            }

            var meshBuilder = GetComponent<SomeMeshBuilder>();
            if (null == meshBuilder)
            {
                Debug.Log("Error. Missing mesh for mesh resizer");
                return;
            }

            var worldSize = Utils.ScreenWorldSize(screenCamera);
            worldSize -= inset * 2.0f;
            meshBuilder.WorldSize = worldSize;
            transform.position = new Vector3(offset.x, offset.y, transform.position.z);
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(ScreenMeshSizer2D))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    var meshSizer = (ScreenMeshSizer2D)target;
                    meshSizer.Build();
                }
            }
        }
#endif
    }
}
