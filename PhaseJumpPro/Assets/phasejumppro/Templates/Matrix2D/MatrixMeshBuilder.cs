using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/7/22
 */
namespace PJ
{
    /// <summary>
    /// Build a Matrix mesh for a Matrix board runner
    /// </summary>
    [RequireComponent(typeof(MatrixBoardRunner2D))]
    public class MatrixMeshBuilder : SomeMeshBuilder
    {
        public float strokeWidth = 0.1f;

        public override Vector2 WorldSize
        {
            get
            {
                var matrixBoardRunner = GetComponent<MatrixBoardRunner2D>();
                if (matrixBoardRunner)
                {
                    return matrixBoardRunner.worldSize;
                }
                return new(1.0f, 1.0f);
            }

            set
            {
                var matrixBoardRunner = GetComponent<MatrixBoardRunner2D>();
                if (matrixBoardRunner)
                {
                    matrixBoardRunner.worldSize = value;
                }
            }
        }

        public Vector2Int GridSize
        {
            get
            {
                var matrixBoardRunner = GetComponent<MatrixBoardRunner2D>();
                if (matrixBoardRunner)
                {
                    return matrixBoardRunner.matrixSize;
                }
                return new(1, 1);
            }
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new GridMesh(GridSize, WorldSize, new Vector2(0, 0), strokeWidth);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(MatrixMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    MatrixMeshBuilder meshBuilder = (MatrixMeshBuilder)target;
                    meshBuilder.Build();
                }
            }
        }
#endif
    }
}
