using System;
using UnityEngine;
using PJ;

/// <summary>
/// Accepts drags of pieces, shows highlight UI for valid moves
/// And lets the board know when a piece should be moved after a drag
/// </summary>
public class CKSquareDropTarget : DropTarget
{
    [NonSerialized]
    public CKBoardView boardView;

    [NonSerialized]
    public Vector2Int location;

    protected override void OnAwake()
    {
        base.OnAwake();
    }

    public override bool CanAcceptDrag(DragItems items)
    {
        return boardView.IsValidMove(items, location);
    }

    public override void OnAcceptDrag(DragItems items)
    {
        base.OnAcceptDrag(items);

        boardView.MovePiece(items.dragHandlers[0].GetComponent<CKPieceNode>().piece, location, 0.3f);
    }
}
