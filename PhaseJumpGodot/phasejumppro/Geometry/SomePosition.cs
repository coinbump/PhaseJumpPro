using Godot;
using System;

/*
RATING: 4 stars
Needs conversion logic
TODO: needs unit tests
CODE REVIEW: 6/8/23
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
        public float x;
        public float y;
        public float z;

        public Vector3 Position
        {
            get => new Vector3(x, y, z);
            set
            {
                x = value.X;
                y = value.Y;
                z = value.Z;
            }
        }

        public WorldPosition(Vector3 value)
        {
            x = value.X;
            y = value.Y;
            z = value.Z;
        }

        public static implicit operator Vector3(WorldPosition position) => position.Position;

        public static WorldPosition operator +(WorldPosition a, WorldPosition b)
        {
            return new WorldPosition(a.Position + b.Position);
        }

        public static WorldPosition operator -(WorldPosition a)
        {
            return new WorldPosition(-a.Position);
        }

        public static WorldPosition operator -(WorldPosition a, WorldPosition b)
        {
            return new WorldPosition(a.Position - b.Position);
        }
    }

    public struct ViewPosition : SomePosition
    {
        public float x;
        public float y;

        public Vector3 Position
        {
            get => new Vector3(x, y, 0);
            set
            {
                x = value.X;
                y = value.Y;
            }
        }

        public ViewPosition(Vector3 value)
        {
            x = value.X;
            y = value.Y;
        }

        public static implicit operator Vector3(ViewPosition position) => position.Position;
        public static implicit operator Vector2(ViewPosition position) => position.Position;

        public static ViewPosition operator +(ViewPosition a, ViewPosition b)
        {
            return new ViewPosition(a.Position + b.Position);
        }

        public static ViewPosition operator -(ViewPosition a)
        {
            return new ViewPosition(-a.Position);
        }

        public static ViewPosition operator -(ViewPosition a, ViewPosition b)
        {
            return new ViewPosition(a.Position - b.Position);
        }
    }

    public struct LocalPosition : SomePosition
    {
        public float x;
        public float y;
        public float z;

        public WeakReference<Node> reference;

        public Vector3 Position
        {
            get => new Vector3(x, y, z);
            set
            {
                x = value.X;
                y = value.Y;
                z = value.Z;
            }
        }

        public LocalPosition(Vector3 value, Node reference)
        {
            x = value.X;
            y = value.Y;
            z = value.Z;
            this.reference = new WeakReference<Node>(reference);
        }

        public static implicit operator Vector3(LocalPosition position) => position.Position;
    }

    public struct ScreenPosition : SomePosition
    {
        public float x;
        public float y;

        public Vector3 Position
        {
            get => new Vector3(x, y, 0);
            set
            {
                x = value.X;
                y = value.Y;
            }
        }

        public ScreenPosition(Vector3 value)
        {
            x = value.X;
            y = value.Y;
        }

        public static implicit operator Vector3(ScreenPosition position) => position.Position;
        public static implicit operator Vector2(ScreenPosition position) => position.Position;

        public static ScreenPosition operator +(ScreenPosition a, ScreenPosition b)
        {
            return new ScreenPosition(a.Position + b.Position);
        }

        public static ScreenPosition operator -(ScreenPosition a)
        {
            return new ScreenPosition(-a.Position);
        }

        public static ScreenPosition operator -(ScreenPosition a, ScreenPosition b)
        {
            return new ScreenPosition(a.Position - b.Position);
        }
    }
}
