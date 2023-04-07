using UnityEngine;
using PJ;
using UnityEngine.InputSystem;

/// <summary>
/// Accepts 4-way movement and moves in NSEW directions
/// </summary>
public class MRHero : MatrixNode2D
{
    public void OnInputMoveLeft(InputValue inputValue)
    {
        transform.parent.GetComponent<MatrixBoardView2D>().MovePiece(piece, MapDirection.West, 0.3f);
    }

    public void OnInputMoveRight(InputValue inputValue)
    {
        transform.parent.GetComponent<MatrixBoardView2D>().MovePiece(piece, MapDirection.East, 0.3f);
    }

    public void OnInputMoveUp(InputValue inputValue)
    {
        transform.parent.GetComponent<MatrixBoardView2D>().MovePiece(piece, MapDirection.North, 0.3f);
    }

    public void OnInputMoveDown(InputValue inputValue)
    {
        transform.parent.GetComponent<MatrixBoardView2D>().MovePiece(piece, MapDirection.South, 0.3f);
    }
}
