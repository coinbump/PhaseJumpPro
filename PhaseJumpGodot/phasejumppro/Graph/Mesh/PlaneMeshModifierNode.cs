using Godot;

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
        public class PlaneMeshModifierNode : GoStandardAcyclicNode
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
                    GD.Print("WARNING. Missing Plane Mesh");
                    return;
                }

                base.Go();

                var map = ValueForInput<SomeTransform<Vector2, float>>("map", null, null) as SomeTransform<Vector2, float>;
                if (null == map)
                {
                    GD.Print("WARNING. Missing Map2D input for map");
                    return;
                }

                planeMesh.Build(true);

                var vertices = planeMesh.mesh.vertices;
                var verticesSize = planeMesh.VerticesSize;
                for (int x = 0; x < verticesSize.X; x++)
                {
                    for (int y = 0; y < verticesSize.Y; y++)
                    {
                        float factorX = (float)x / (float)(verticesSize.X - 1);
                        float factorY = (float)y / (float)(verticesSize.Y - 1);
                        if (flipY)
                        {
                            factorY = 1.0f - factorY;
                        }

                        float value = map.ValueFor(new Vector2(factorX, factorY)) * scale;

                        int index = y * verticesSize.X + x;
                        Debug.Assert(index < vertices.Length);
                        var vertex = vertices[index];

                        var newVertex = new Vector3(vertex.X, vertex.Y, vertex.Z);
                        switch (modifierAxis)
                        {
                            case Axis.X:
                                newVertex = new Vector3(vertex.X + value, vertex.Y, vertex.Z);
                                break;
                            case Axis.Y:
                                newVertex = new Vector3(vertex.X, vertex.Y + value, vertex.Z);
                                break;
                            case Axis.Z:
                                newVertex = new Vector3(vertex.X, vertex.Y, vertex.Z + value);
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
