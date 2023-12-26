using Godot;

/*
 * RATING: 5 stars
 * Utility class
 * CODE REVIEW: 3/13/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Utility to speed up building a serial node graph with standard types
        /// </summary>
        public class SerialBuilder
        {
            protected GoStandardAcyclicNode node;

            public SerialBuilder(GoStandardAcyclicNode node)
            {
                this.node = node;
            }

            public SerialBuilder MultiplyBy(SomeMap2DNode map2)
            {
                var multiplyNode = new MultiplyMaps2DNode();
                node.AddOutput("value", multiplyNode, "map-1");
                map2.AddOutput("value", multiplyNode, "map-2");

                return new SerialBuilder(multiplyNode);
            }

            public SerialBuilder OutputTo(string outputId, GoStandardAcyclicNode toNode, string inputId)
            {
                node.AddOutput(outputId, toNode, inputId);
                return new SerialBuilder(toNode);
            }

            public SerialBuilder ToMap2D(Axis2D axis)
            {
                var transformNode = new Map1DToMap2DNode(axis);
                node.AddOutput("value", transformNode, "value");
                return new SerialBuilder(transformNode);
            }
        }
    }
}
