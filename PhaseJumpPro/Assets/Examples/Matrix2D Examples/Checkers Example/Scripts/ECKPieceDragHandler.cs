using UnityEngine;
using PJ;

/// <summary>
/// Drag handler for checkers piece
/// Snaps back unless the piece is being moved to a new position
/// </summary>
public class ECKPieceDragHandler : DragHandler2D
{
    public override void OnDragEnd()
    {
        base.OnDragEnd();

        if (GetComponent<MatrixNode2D>().IsAnimating)
        {
            return;
        }

        // Snap back
        transform.position = dragStartPosition.Position;
    }
}
