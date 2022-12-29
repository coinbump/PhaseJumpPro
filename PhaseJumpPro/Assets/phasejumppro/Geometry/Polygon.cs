using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/3/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Stores vertices for closed polygon
    /// </summary>
    [System.Serializable]
    public class Polygon
    {
        public List<Vector3> vertices = new List<Vector3>();

        public Vector3 Min
        {
            get
            {
                if (vertices.Count == 0) { return Vector3.zero; }

                var result = vertices[0];

                foreach (var vertex in vertices)
                {
                    result = new Vector3(Mathf.Min(result.x, vertex.x), Mathf.Min(result.y, vertex.y), Mathf.Min(result.z, vertex.z));
                }

                return result;
            }
        }

        public Vector3 Max
        {
            get
            {
                if (vertices.Count == 0) { return Vector3.zero; }

                var result = vertices[0];

                foreach (var vertex in vertices)
                {
                    result = new Vector3(Mathf.Max(result.x, vertex.x), Mathf.Max(result.y, vertex.y), Mathf.Max(result.z, vertex.z));
                }

                return result;
            }
        }

        public Vector3 Size
        {
            get
            {
                var min = Min;
                var max = Max;

                return new Vector3(Mathf.Abs(max.x - min.x), Mathf.Abs(max.y - min.y), Mathf.Abs(max.z - min.z));
            }
        }

        public Vector3 Center
        {
            get
            {
                var min = Min;
                var size = Size;

                return new Vector3(min.x + size.x / 2.0f, min.y + size.y / 2.0f, min.z + size.z / 2.0f);
            }
        }

        public void Inset2D(Vector2 inset)
        {
            var center = Center;

            List<Vector3> result = new();
            foreach (var iterVertex in vertices)
            {
                var vertex = iterVertex;

                if (vertex.x < center.x)
                {
                    vertex.x += inset.x;
                }
                else
                {
                    vertex.x -= inset.x;
                }

                if (vertex.y < center.y)
                {
                    vertex.y += inset.y * Vector2.up.y;
                }
                else
                {
                    vertex.y += inset.y * Vector2.down.y;
                }

                result.Add(vertex);
            }

            vertices = result;
        }
    }
}
