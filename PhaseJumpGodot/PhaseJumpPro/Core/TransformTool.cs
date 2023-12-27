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
