using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;

/// <summary>
/// Uses Matrix Board View to play basic Checkers concept (not the entire game)
/// How to Play: Drag pieces forward or over an opponent's piece to jump (no double jumps)
/// </summary>
public class CKBoardView : MatrixBoardView2D
{
    protected SceneObjectRegistry sceneObjectRegistry;

    protected override void Awake()
    {
        base.Awake();

        sceneObjectRegistry = FindObjectOfType<SceneObjectRegistry>();
    }

    protected override void Start()
    {
        base.Start();

        BuildCheckerboard();
        PopulatePieces();
    }

    protected void BuildCheckerboard()
    {
        bool isDark = false;
        var cellCount = matrixSize.x * matrixSize.y;
        var cellSize = CellSize;

        for (int y = 0; y < matrixSize.y; y++)
        {
            for (int x = 0; x < matrixSize.x; x++)
            {
                var rootX = -(WorldSize.x / 2.0f) + (cellSize.x / 2.0f);
                var xPos = rootX + x * cellSize.x;
                var rootY = (WorldSize.y / 2.0f) * Vector2.up.y + (cellSize.y / 2.0f) * Vector2.down.y;
                var yPos = rootY + y * cellSize.y * Vector2.down.y;

                var pos = new Vector3(xPos, yPos, 2);

                var id = isDark ? "square-dark" : "square-light";
                var gameObject = sceneObjectRegistry.InstantiateGameObject(id, pos, Quaternion.identity);
                gameObject.transform.parent = transform;
                gameObject.transform.localPosition = pos;

                if (isDark)
                {
                    var dropTarget = gameObject.GetComponent<CKSquareDropTarget>();
                    dropTarget.boardView = this;
                    dropTarget.location = new Vector2Int(x, y);
                }

                isDark = !isDark;
            }

            isDark = !isDark;
        }
    }

    void PopulatePieces()
    {
        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < matrixSize.x; x++)
            {
                var location = new Vector2Int(x, y);
                if (!IsDarkSquareAt(location)) { continue; }

                var pieceObject = sceneObjectRegistry.InstantiateGameObject("piece-black", Vector3.zero, Quaternion.identity);
                var piece = pieceObject.GetComponent<CKPieceNode>();

                // Hide the king overlay
                foreach (Transform childTransform in pieceObject.transform)
                {
                    childTransform.GetComponent<SpriteRenderer>().enabled = false;
                }

                Put(piece, location);
            }
        }

        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < matrixSize.x; x++)
            {
                var location = new Vector2Int(x, matrixSize.y - 1 - y);
                if (!IsDarkSquareAt(location)) { continue; }

                var pieceObject = sceneObjectRegistry.InstantiateGameObject("piece-red", Vector3.zero, Quaternion.identity);
                var piece = pieceObject.GetComponent<CKPieceNode>();

                // Hide the king overlay
                foreach (Transform childTransform in pieceObject.transform)
                {
                    childTransform.GetComponent<SpriteRenderer>().enabled = false;
                }

                Put(piece, location);
            }
        }
    }

    bool IsDarkSquareAt(Vector2Int location)
    {
        var isYEven = location.y % 2.0f == 0;
        var isXEven = location.x % 2.0f == 0;
        return isYEven ? !isXEven : isXEven;
    }

    public bool IsValidMove(DragItems dragItems, Vector2Int location)
    {
        var pieceComponent = dragItems.dragHandlers[0].gameObject.GetComponent<CKPieceNode>();
        var pieceLocation = pieceComponent.piece.origin;

        if (board.IsCellBlockedAt(location)) { return false; }

        switch (pieceComponent.isBlack)
        {
            case true:
                // Ok to move 1 diagonal.
                if (location.y - pieceLocation.y == 1)
                {
                    return Mathf.Abs(location.x - pieceLocation.x) == 1;
                }

                // In this example, no double jumps are allowed.
                if (location.y - pieceLocation.y == 2 && Mathf.Abs(location.x - pieceLocation.x) == 2)
                {
                    var jumpedLocation = new Vector2Int(pieceLocation.x + (location.x - pieceLocation.x) / 2, pieceLocation.y + 1);
                    var jumpedPieceNode = GameObjectAt(jumpedLocation);
                    if (jumpedPieceNode)
                    {
                        var jumpedPieceComponent = jumpedPieceNode.GetComponent<CKPieceNode>();
                        if (jumpedPieceComponent.isBlack != pieceComponent.isBlack)
                        {
                            return true;
                        }
                    }
                }
                break;
            case false:
                if (location.y - pieceLocation.y == -1)
                {
                    return Mathf.Abs(location.x - pieceLocation.x) == 1;
                }

                // In this example, no double jumps are allowed.
                if (location.y - pieceLocation.y == -2 && Mathf.Abs(location.x - pieceLocation.x) == 2)
                {
                    var jumpedLocation = new Vector2Int(pieceLocation.x + (location.x - pieceLocation.x) / 2, pieceLocation.y - 1);
                    var jumpedPieceNode = GameObjectAt(jumpedLocation);
                    if (jumpedPieceNode)
                    {
                        var jumpedPieceComponent = jumpedPieceNode.GetComponent<CKPieceNode>();
                        if (jumpedPieceComponent.isBlack != pieceComponent.isBlack)
                        {
                            return true;
                        }
                    }
                }
                break;
        }

        return false;
    }

    public override MoveResult MovePiece(MatrixPiece piece, Vector2Int newOrigin, float duration)
    {
        var pieceLocation = piece.origin;
        var location = newOrigin;

        if (base.MovePiece(piece, newOrigin, duration) != MoveResult.Success) { return MoveResult.Fail; }

        // Animate the jumped piece offscreen and cull it when it becomes invisible
        if (Mathf.Abs(location.y - pieceLocation.y) == 2 && Mathf.Abs(location.x - pieceLocation.x) == 2)
        {
            var jumpedLocation = new Vector2Int(pieceLocation.x + (location.x - pieceLocation.x) / 2, pieceLocation.y + (location.y - pieceLocation.y) / 2);
            var jumpedPieceNode = GameObjectAt(jumpedLocation);

            RemoveAt(jumpedLocation, false);
            jumpedPieceNode.transform.parent = null;
            jumpedPieceNode.GetComponent<SomeNode>().cullTypes.Add(NodeCullType.Invisible);

            var screenWorldSize = Utils.ScreenWorldSize(Camera);
            var destinationX = (-screenWorldSize.x / 2.0f) - 2.0f;
            var destinationY = UnityEngine.Random.Range(0, screenWorldSize.y) + screenWorldSize.y / 2.0f * Vector2.down.y;

            var animator = new CycleAnimator<Vector3>(
                new Vector3Interpolator(jumpedPieceNode.transform.position, new Vector3(destinationX, destinationY, 0), new EaseInCubed()),
                new(0.4f, AnimationCycleType.Once),
                new SetBinding<Vector3>((Vector3 value) => jumpedPieceNode.transform.position = value)
            );

            updatables.Add(animator);
        }

        return MoveResult.Success;
    }
}
