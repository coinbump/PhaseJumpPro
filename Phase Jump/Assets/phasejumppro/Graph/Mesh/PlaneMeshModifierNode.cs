using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 3/13/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Modifies the mesh vertices based on a Map2D input.
        /// Useful for warping meshes based on noise input (terrain)
        /// </summary>
        public class PlaneMeshModifierNode : GoStandardNode
        {
            public PlaneMesh planeMesh;
            public Axis modifierAxis = Axis.Z;
            public float scale = 1.0f;
            public bool flipY = false;
           
            public PlaneMeshModifierNode(PlaneMesh planeMesh, Axis modifierAxis, float scale, bool flipY)
            {
                this.planeMesh = planeMesh;
                this.modifierAxis = modifierAxis;
                this.scale = scale;
                this.flipY = flipY;
            }

            public override void Go()
            {
                if (null == planeMesh)
                {
                    Debug.Log("WARNING. Missing Plane Mesh");
                    return;
                }

                base.Go();

                var map = ValueForInput<SomeMap<Vector2, float>>("map", null, null) as SomeMap<Vector2, float>;
                if (null == map)
                {
                    Debug.Log("WARNING. Missing Map2D input for map");
                    return;
                }
                
                planeMesh.Build(true);

                var vertices = planeMesh.mesh.vertices;
                var verticesSize = planeMesh.VerticesSize;
                for (int x = 0; x < verticesSize.x; x++)
                {
                    for (int y = 0; y < verticesSize.y; y++)
                    {
                        float factorX = (float)x / (float)(verticesSize.x - 1);
                        float factorY = (float)y / (float)(verticesSize.y - 1);
                        if (flipY)
                        {
                            factorY = 1.0f - factorY;
                        }

                        float value = map.ValueFor(new Vector2(factorX, factorY)) * scale;

                        int index = y * verticesSize.x + x;
                        Debug.Assert(index < vertices.Length);
                        var vertex = vertices[index];

                        var newVertex = new Vector3(vertex.x, vertex.y, vertex.z);
                        switch (modifierAxis)
                        {
                            case Axis.X:
                                newVertex = new Vector3(vertex.x + value, vertex.y, vertex.z);
                                break;
                            case Axis.Y:
                                newVertex = new Vector3(vertex.x, vertex.y + value, vertex.z);
                                break;
                            case Axis.Z:
                                newVertex = new Vector3(vertex.x, vertex.y, vertex.z + value);
                                break;
                        }

                        vertices[index] = newVertex;
                    }
                }

                planeMesh.Vertices = vertices;
            }
        }
    }
}
