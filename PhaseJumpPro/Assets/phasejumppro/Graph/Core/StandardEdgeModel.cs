/*
 * RATING: 5 stars
 * Simple model type
 * CODE REVIEW: 3/6/22
 *
 * PORTED TO: C++
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Default implementation of an edge model. Or you can create your own
        /// </summary>
        public struct StandardEdgeModel
        {
            public string id; // Example: north, west, left, right
            public string type; // Example: input, output
            public float weight;
            public Tags tags;

            public StandardEdgeModel(string id, string type, float weight = 1.0f, Tags tags = null)
            {
                this.id = id;
                this.type = type;
                this.weight = weight;

                if (null != tags)
                {
                    this.tags = tags;
                }
                else
                {
                    this.tags = new Tags();
                }
            }

            public string Id => id;
            public string Type => type;
            public float Weight => weight;
        }
    }
}
