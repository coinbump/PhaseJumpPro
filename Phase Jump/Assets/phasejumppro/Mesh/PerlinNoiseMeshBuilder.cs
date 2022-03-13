using UnityEditor;
using UnityEngine;
using PJ;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 3/13/22
 */
namespace PJ
{
    /// <summary>
    /// Warp a plane mesh with perlin noise
    /// </summary>
    public class PerlinNoiseMeshBuilder : SomeMeshBuilder
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Axis faceAxis = Axis.Z;
        public Vector2 perlinOffset = Vector2.zero;
        public Vector2 perlinScale = new Vector2(1.0f, 1.0f);
        public Axis modifierAxis = Axis.Z;
        public float modifierScale = 1.0f;

        protected float offset = 0;

        public override Mesh BuildMesh()
        {
            var warpMap = new Graph.PerlinNoiseNode(perlinOffset, perlinScale);
            var graphBuilder = new Graph.SerialBuilder(warpMap);
            var modifierNode = new Graph.PlaneMeshModifierNode(new PlaneMesh(meshSize, worldSize, faceAxis), modifierAxis, modifierScale, true);

            graphBuilder.OutputTo("value", modifierNode, "map");

            modifierNode.Go();
            return modifierNode.planeMesh.mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(PerlinNoiseMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    SomeMeshBuilder planeMeshBuilder = (SomeMeshBuilder)target;
                    planeMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
