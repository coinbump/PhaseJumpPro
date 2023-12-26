using System.Collections.Generic;
using Godot;

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
    public class Polygon : List<Vector3>
    {
        public Vector3 Min
        {
            get
            {
                if (this.Count == 0) { return Vector3.Zero; }

                var result = this[0];

                foreach (var vertex in this)
                {
                    result = new Vector3(Mathf.Min(result.X, vertex.X), Mathf.Min(result.Y, vertex.Y), Mathf.Min(result.Z, vertex.Z));
                }

                return result;
            }
        }

        public Vector3 Max
        {
            get
            {
                if (this.Count == 0) { return Vector3.Zero; }

                var result = this[0];

                foreach (var vertex in this)
                {
                    result = new Vector3(Mathf.Max(result.X, vertex.X), Mathf.Max(result.Y, vertex.Y), Mathf.Max(result.Z, vertex.Z));
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

                return new Vector3(Mathf.Abs(max.X - min.X), Mathf.Abs(max.Y - min.Y), Mathf.Abs(max.Z - min.Z));
            }
        }

        public Vector3 Center
        {
            get
            {
                var min = Min;
                var size = Size;

                return new Vector3(min.X + size.X / 2.0f, min.Y + size.Y / 2.0f, min.Z + size.Z / 2.0f);
            }
        }

        // public void Inset2D(Vector2 inset)
        // {
        //     var center = Center;

        //     List<Vector3> result = new();
        //     foreach (var iterVertex in this)
        //     {
        //         var vertex = iterVertex;

        //         if (vertex.X < center.X)
        //         {
        //             vertex.X += inset.X;
        //         }
        //         else
        //         {
        //             vertex.X -= inset.X;
        //         }

        //         if (vertex.Y < center.Y)
        //         {
        //             vertex.Y += inset.Y * Vector2.up.Y;
        //         }
        //         else
        //         {
        //             vertex.Y += inset.Y * Vector2.down.Y;
        //         }

        //         result.Add(vertex);
        //     }

        //     this = result;
        // }
    }
}
