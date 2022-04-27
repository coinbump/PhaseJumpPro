using System;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/26/22
 */
namespace PJ
{
    /// <summary>
    /// Builds a mesh that matches the angle range of the attached ScanSensor2D
    /// </summary>
    public class ScanMeshBuilder2D : SomeMeshBuilder
    {
        public float angleStep = 10.0f;
        
        public override Vector2 WorldSize
        {
            get
            {
                var circleCollider = GetComponent<CircleCollider2D>();
                if (null == circleCollider) { return Vector2.zero; }

                return new Vector2(circleCollider.radius * 2.0f, circleCollider.radius * 2.0f);
            }
            set
            {
                var circleCollider = GetComponent<CircleCollider2D>();
                if (null == circleCollider) { return; }

                circleCollider.radius = Mathf.Min(value.x, value.y) / 2.0f;
            }
        }

        public override Mesh BuildMesh()
        {
            var scanSensor = GetComponent<ScanSensor2D>();
            if (null == scanSensor) {
                Debug.Log("Error. Scan sensor required for Scan Mesh");
                return null;
            }

            var shapeMesh = new ArcMesh(-scanSensor.degreeAngleScan / 2.0f, scanSensor.degreeAngleScan / 2.0f, angleStep, WorldSize);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(ScanMeshBuilder2D))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    ScanMeshBuilder2D meshBuilder = (ScanMeshBuilder2D)target;
                    meshBuilder.Build();
                }
            }
        }
#endif
    }
}
