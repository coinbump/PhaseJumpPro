using System;
using UnityEngine;
using PJ;

/// <summary>
/// Accepts drags of pieces, shows highlight UI for valid moves
/// And lets the board know when a piece should be moved after a drag
/// </summary>
public class ECKSquareDropTarget : DropTarget
{
    [NonSerialized]
    public ECKBoardView boardView;

    [NonSerialized]
    public Vector2Int location;

    public override bool CanAcceptDrag(DragItems items)
    {
        return boardView.IsValidMove(items, location);
    }

    public override void OnAcceptDrag(DragItems items)
    {
        base.OnAcceptDrag(items);

        boardView.MovePiece(items.dragHandlers[0].GetComponent<ECKPieceNode>().piece, location, 0.3f);
    }
}
