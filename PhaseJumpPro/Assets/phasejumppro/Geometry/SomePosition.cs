using UnityEngine;
using System;

/*
RATING: 4 stars
Needs conversion logic
CODE REVIEW: 12/25/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Encapsulates both position and coordinates used (world/screen/local)
    /// </summary>
    public interface SomePosition
    {
        public Vector3 Position { get; set; }
    }

    public struct WorldPosition : SomePosition
    {
        public Vector3 position;
        public Vector3 Position
        {
            get => position;
            set => position = value;
        }

        public WorldPosition(Vector3 position)
        {
            this.position = position;
        }
    }

    public struct LocalPosition : SomePosition
    {
        public Vector3 position;
        public GameObject reference;

        public Vector3 Position
        {
            get => position;
            set => position = value;
        }

        public LocalPosition(Vector3 position, GameObject reference)
        {
            this.position = position;
            this.reference = reference;
        }
    }

    public struct ScreenPosition : SomePosition
    {
        public Vector2 position;

        public Vector3 Position
        {
            get => new Vector3(position.x, position.y, 0);
            set => position = new Vector2(value.x, value.y);
        }

        public ScreenPosition(Vector2 position)
        {
            this.position = position;
        }
    }
}
