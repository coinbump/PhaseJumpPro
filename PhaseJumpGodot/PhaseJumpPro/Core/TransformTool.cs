using Godot;

namespace PJ;

/// <summary>
/// Handles altering transform for either Node2D or Node3D
/// </summary>
public struct TransformTool
{
    public Node node;

    public TransformTool(Node node)
    {
        this.node = node;
    }

    public Vector3 LocalPosition
    {
        get
        {
            if (node is Node2D node2D)
            {
                return new(node2D.Position.X, node2D.Position.Y, 0);
            }
            else if (node is Node3D node3D)
            {
                return node3D.Position;
            }

            return Vector3.Zero;
        }
        set
        {
            SetLocalPosition(value);
        }
    }

    public Vector3 GlobalPosition
    {
        get
        {
            if (node is Node2D node2D)
            {
                return new(node2D.GlobalPosition.X, node2D.GlobalPosition.Y, 0);
            }
            else if (node is Node3D node3D)
            {
                return node3D.GlobalPosition;
            }

            return Vector3.Zero;
        }
        set
        {
            SetGlobalPosition(value);
        }
    }

    public readonly void SetLocalPosition(Vector3 position)
    {
        if (node is Node2D node2D)
        {
            node2D.Position = new(position.X, position.Y);
        }
        else if (node is Node3D node3D)
        {
            node3D.Position = position;
        }
    }

    public readonly void SetGlobalPosition(Vector3 position)
    {
        if (node is Node2D node2D)
        {
            node2D.GlobalPosition = new(position.X, position.Y);
        }
        else if (node is Node3D node3D)
        {
            node3D.GlobalPosition = position;
        }
    }
}
