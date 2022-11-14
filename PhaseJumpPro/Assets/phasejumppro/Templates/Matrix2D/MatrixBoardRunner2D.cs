using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 STARS
 * TESTED AND WORKS
 * CODE REVIEW: 4/24/22
 */
namespace PJ
{
    /// <summary>
    /// Attach this to a gameObject whose children are MatrixNode2D nodes and are
    /// locked to matrix movement (staying within cells).
    /// When moving between cells, a move animation can give the impression of free movement
    /// </summary>
    public class MatrixBoardRunner2D : MonoBehaviour
    {
        public enum MoveResult
        {
            // Can't move, it's animating
            Animating,

            // Success
            Success,

            // Destination is blocked
            Blocked,

            // Error occurred
            Fail
        }

        public Vector2Int matrixSize = new Vector2Int(5, 5);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        
        public MatrixBoard board;

        [Tooltip("Optional focus indicator")]
        public GameObject focusIndicator;

        protected Optional<Vector2Int> mouseFocusedCell;
        protected MouseInputController mouseInputController = new();

        public Vector2 TopLeftWorldPosition
        {
            get
            {
                var origin = transform.position;
                var topLeft = new Vector2(
                    origin.x - worldSize.x / 2.0f,
                    origin.y + worldSize.y / 2.0f * Vector2.up.y
                );
                return topLeft;
            }
        }

        protected override void Awake()
        {
            board = new MatrixBoard(matrixSize);
        }

        protected override void Start()
        {
            foreach (Transform childTransform in transform)
            {
                var childNode = childTransform.gameObject.GetComponent<MatrixNode2D>();
                if (null == childNode) { continue; }
                if (null == childNode.piece) {
                    Debug.Log("Error. Matrix node is missing a piece");
                    continue;
                }

                var putResult = board.PutPiece(childNode.piece, childNode.origin);
                if (!putResult)
                {
                    Debug.Log("Error. Can't place Matrix node at " + childNode.origin.ToString());
                }

                var nodePosition = NodePosition(childNode);
                childTransform.localPosition = nodePosition;
            }
        }

        protected override void Update()
        {
            if (mouseInputController.IsAvailable())
            {
                var worldPosition = mouseInputController.WorldPosition;
                if (null == worldPosition) { return; }

                mouseFocusedCell = CellAtWorldPosition(new Vector2(worldPosition.value.x, worldPosition.value.y));

                if (focusIndicator)
                {
                    if (null != mouseFocusedCell)
                    {
                        var cellPosition = CellToWorldPosition(mouseFocusedCell.value);

                        focusIndicator.transform.position = new Vector3(cellPosition.x, cellPosition.y, focusIndicator.transform.position.z);
                        var meshRenderer = focusIndicator.GetComponent<MeshRenderer>();
                        if (meshRenderer)
                        {
                            meshRenderer.enabled = true;
                        }
                    }
                    else
                    {
                        var meshRenderer = focusIndicator.GetComponent<MeshRenderer>();
                        if (meshRenderer)
                        {
                            meshRenderer.enabled = false;
                        }
                    }
                }
                // Debug.Log(mouseFocusedCell.ToString());
            }
        }

        public Vector2 CellSize
        {
            get
            {
                return worldSize/(new Vector2(matrixSize.x, matrixSize.y));
            }
        }

        public bool IsReadingPositionInside(Vector2 readingPosition)
        {
            if (readingPosition.x < 0 || readingPosition.y < 0) { return false; }
            if (readingPosition.x > worldSize.x || readingPosition.y > worldSize.y) { return false; }

            return true;
        }

        public Optional<Vector2Int> CellAtWorldPosition(Vector2 worldPosition)
        {
            var topLeft = TopLeftWorldPosition;

            // The Matrix works in right-hand coordinate space
            var readingPosition = new Vector2(worldPosition.x - topLeft.x, Mathf.Abs(worldPosition.y - topLeft.y));
            if (!IsReadingPositionInside(readingPosition)) { return null; }

            var cell = CellAtReadingPosition(readingPosition);
            return new Optional<Vector2Int>(cell);
        }

        public Vector2Int CellAtReadingPosition(Vector2 readingPosition)
        {
            var column = (int)(readingPosition.x / CellSize.x);
            var row = (int)(readingPosition.y / CellSize.y);

            return new Vector2Int(column, row);
        }

        public Vector2 CellToLocalPosition(Vector2Int cell)
        {
            var x = -(worldSize.x / 2.0f) + CellSize.x / 2.0f + cell.x * CellSize.x;
            var y = (worldSize.y / 2.0f) * Vector2.up.y + (CellSize.y / 2.0f + cell.y * CellSize.y) * Vector2.down.y;

            return new Vector2(x, y);
        }

        public Vector3 CellToWorldPosition(Vector2Int cell)
        {
            var topLeft = TopLeftWorldPosition;
            var x = topLeft.x + CellSize.x / 2.0f + cell.x * CellSize.x;
            var y = topLeft.y + (CellSize.y / 2.0f + cell.y * CellSize.y) * Vector2.down.y;

            return new Vector3(x, y, 0);
        }

        public MoveResult MovePiece(MatrixPiece piece, MapDirection direction, float duration)
        {
            if (null == piece) { return MoveResult.Fail; }

            if (null == piece.owner || !piece.owner.TryGetTarget(out object pieceOwner))
            {
                Debug.Log("Error. Piece owner is missing");
                return MoveResult.Fail;
            }

            MatrixNode2D node = pieceOwner as MatrixNode2D;
            if (null == node)
            {
                Debug.Log("Error. Need MatrixNode2D for Matrix board");
                return MoveResult.Fail;
            }

            if (node.IsAnimating) { return MoveResult.Animating; }

            var oldOrigin = piece.origin;
            var newOrigin = oldOrigin + direction.Offset();

            if (newOrigin == oldOrigin) { return MoveResult.Fail; }

            var excludeList = new HashSet<MatrixPiece>();
            excludeList.Add(piece);
            if (board.IsPieceBlockedAt(newOrigin, piece, excludeList))
            {
                return MoveResult.Blocked;
            }

            board.RemovePiece(piece);
            board.PutPiece(piece, newOrigin);

            var endPosition = NodePosition(node);

            if (duration > 0)
            {
                node.moveAnimationCurve = new Optional<AnimationCurve<Vector2>>(new AnimationCurve<Vector2>());
                node.moveAnimationCurve.value.start = node.transform.localPosition;
                node.moveAnimationCurve.value.end = endPosition;

                node.moveAnimationTimer.duration = duration;
                node.moveAnimationTimer.Reset();
            }
            else
            {
                node.transform.localPosition = endPosition;
            }

            return MoveResult.Success;
        }

        public Vector2 NodePosition(MatrixNode2D node)
        {
            var pieceOrigin = node.piece.origin;
            var pieceSize = node.piece.Size;

            var topLeftCellPosition = CellToLocalPosition(pieceOrigin);
            var bottomRightCellPosition = CellToLocalPosition(pieceOrigin + new Vector2Int(pieceSize.x - 1, pieceSize.y -1));

            var x = topLeftCellPosition.x + (bottomRightCellPosition.x - topLeftCellPosition.x) / 2.0f;
            var y = topLeftCellPosition.y + Mathf.Abs((bottomRightCellPosition.y - topLeftCellPosition.y) / 2.0f) * Vector2.down.y;

            return new Vector2(x, y);
        }
    }    
}
